
#ifndef STB_INCLUDE_STB_RECT_PACK_H
#define STB_INCLUDE_STB_RECT_PACK_H

#define STBRP_MAXVAL 0x0000FFFF

typedef struct stbrp_context stbrp_context;
typedef struct stbrp_node stbrp_node;
typedef struct stbrp_rect stbrp_rect;
typedef unsigned short stbrp_coord;

void stbrp_pack_rects(stbrp_context *, stbrp_rect *, int);

struct stbrp_rect {
	int id;
	stbrp_coord w, h;
	stbrp_coord x, y;
	int was_packed;
};

void stbrp_init_target(stbrp_context *, int, int, stbrp_node *, int);
void stbrp_setup_allow_out_of_mem(stbrp_context *, int);
void stbrp_setup_heuristic(stbrp_context *, int);

enum {
	STBRP_HEURISTIC_Skyline_default = 0, STBRP_HEURISTIC_Skyline_BL_sortHeight = STBRP_HEURISTIC_Skyline_default, STBRP_HEURISTIC_Skyline_BF_sortHeight,
};

struct stbrp_node {
	stbrp_coord x, y;
	stbrp_node * next;
};

struct stbrp_context {
	int width;
	int height;
	int align;
	int init_mode;
	int heuristic;
	int num_nodes;
	stbrp_node * active_head;
	stbrp_node * free_head;
	stbrp_node extra[2];
};

#endif

#ifdef STB_RECT_PACK_IMPLEMENTATION

enum {
	STBRP__INIT_skyline = 1,
};

void stbrp_setup_heuristic(stbrp_context * context, int heuristic) {
	if (context->init_mode == STBRP__INIT_skyline) context->heuristic = heuristic;
};

void stbrp_setup_allow_out_of_mem(stbrp_context * context, int allow_out_of_mem) {
	if (allow_out_of_mem) context->align = 1;
	else context->align = (context->width + context->num_nodes - 1) / context->num_nodes;
};

void stbrp_init_target(stbrp_context * context, int width, int height, stbrp_node * nodes, int num_nodes)
{
	static int i = 0;
	for (i = 0; i < num_nodes - 1; ++i) nodes[i].next = &nodes[i + 1];
	nodes[i].next = NULL;
	context->init_mode = STBRP__INIT_skyline;
	context->heuristic = STBRP_HEURISTIC_Skyline_default;
	context->free_head = &nodes[0];
	context->active_head = &context->extra[0];
	context->width = width;
	context->height = height;
	context->num_nodes = num_nodes;
	stbrp_setup_allow_out_of_mem(context, 0);
	context->extra[0].x = 0;
	context->extra[0].y = 0;
	context->extra[0].next = &context->extra[1];
	context->extra[1].x = (stbrp_coord)width;
	context->extra[1].y = 65535;
	context->extra[1].next = NULL;
}

int stbrp__skyline_find_min_y(stbrp_context * c, stbrp_node * first, int x0, int width, int * pwaste)
{
	stbrp_node * node = first;
	int x1 = x0 + width, min_y, visited_width, waste_area;
	min_y = waste_area = visited_width = 0;
	while (node->x < x1) {
		if (node->y > min_y) {
			waste_area += visited_width * (node->y - min_y);
			min_y = node->y;
			if (node->x < x0) visited_width += node->next->x - x0;
			else visited_width += node->next->x - node->x;
		}
		else {
			int under_width = node->next->x - node->x;
			if (under_width + visited_width > width) under_width = width - visited_width;
			waste_area += under_width * (min_y - node->y);
			visited_width += under_width;
		};
		node = node->next;
	};
	*pwaste = waste_area;
	return min_y;
};

typedef struct {
	int x, y;
	stbrp_node ** prev_link;
} stbrp__findresult;

stbrp__findresult stbrp__skyline_find_best_pos(stbrp_context * c, int width, int height) {
	int best_waste = (1 << 30), best_x, best_y = (1 << 30);
	stbrp__findresult fr;
	stbrp_node ** prev, *node, *tail, ** best = NULL;
	width = (width + c->align - 1), width -= width % c->align;
	node = c->active_head, prev = &c->active_head;
	while (node->x + width <= c->width) {
		int y, waste;
		y = stbrp__skyline_find_min_y(c, node, node->x, width, &waste);
		if (c->heuristic == STBRP_HEURISTIC_Skyline_BL_sortHeight) {
			if (y < best_y) {
				best_y = y;
				best = prev;
			}
		}
		else {
			if (y + height <= c->height) {
				if (y < best_y || (y == best_y && waste < best_waste)) {
					best_y = y;
					best_waste = waste;
					best = prev;
				}
			}
		}
		prev = &node->next, node = node->next;
	}
	best_x = (best == NULL) ? 0 : (*best)->x;
	if (c->heuristic == STBRP_HEURISTIC_Skyline_BF_sortHeight) {
		tail = c->active_head;
		node = c->active_head;
		prev = &c->active_head;
		while (tail->x < width) tail = tail->next;
		while (tail) {
			int xpos = tail->x - width, y, waste;
			while (node->next->x <= xpos) {
				prev = &node->next;
				node = node->next;
			}
			y = stbrp__skyline_find_min_y(c, node, xpos, width, &waste);
			if (y + height < c->height) {
				if (y <= best_y) {
					if (y < best_y || waste < best_waste || (waste == best_waste && xpos < best_x)) {
						best_x = xpos;
						best_y = y;
						best_waste = waste;
						best = prev;
					}
				}
			}
			tail = tail->next;
		}
	}
	fr.prev_link = best;
	fr.x = best_x;
	fr.y = best_y;
	return fr;
}

stbrp__findresult stbrp__skyline_pack_rectangle(stbrp_context * context, int width, int height) {
	stbrp__findresult res = stbrp__skyline_find_best_pos(context, width, height);
	stbrp_node * node, *cur;
	if (res.prev_link == NULL || res.y + height > context->height || context->free_head == NULL) {
		res.prev_link = NULL;
		return res;
	}
	node = context->free_head;
	node->x = (stbrp_coord)res.x;
	node->y = (stbrp_coord)(res.y + height);
	context->free_head = node->next;
	cur = *res.prev_link;
	if (cur->x < res.x) {
		stbrp_node *next = cur->next;
		cur->next = node;
		cur = next;
	}
	else {
		*res.prev_link = node;
	}
	while (cur->next && cur->next->x <= res.x + width) {
		stbrp_node *next = cur->next;
		cur->next = context->free_head;
		context->free_head = cur;
		cur = next;
	}
	node->next = cur;
	if (cur->x < res.x + width) cur->x = (stbrp_coord)(res.x + width);
	return res;
}

int rect_height_compare(const void * a, const void * b)
{
	const stbrp_rect *p = (const stbrp_rect *)a;
	const stbrp_rect *q = (const stbrp_rect *)b;
	if (p->h > q->h) return -1;
	if (p->h < q->h) return 1;
	return (p->w > q->w) ? -1 : (p->w < q->w);
}

int rect_width_compare(const void * a, const void * b)
{
	const stbrp_rect *p = (const stbrp_rect *)a;
	const stbrp_rect *q = (const stbrp_rect *)b;
	if (p->w > q->w) return -1;
	if (p->w < q->w) return 1;
	return (p->h > q->h) ? -1 : (p->h < q->h);
}

int rect_original_order(const void * a, const void * b)
{
	const stbrp_rect *p = (const stbrp_rect *)a;
	const stbrp_rect *q = (const stbrp_rect *)b;
	return (p->was_packed < q->was_packed) ? -1 : (p->was_packed > q->was_packed);
}

void stbrp_pack_rects(stbrp_context * context, stbrp_rect * rects, int num_rects)
{
	static int i = 0;
	for (i = 0; i < num_rects; ++i) rects[i].was_packed = i;
	qsort(rects, num_rects, sizeof(rects[0]), rect_height_compare);
	for (i = 0; i < num_rects; ++i) {
		if (rects[i].w == 0 || rects[i].h == 0) {
			rects[i].x = rects[i].y = 0;
		}
		else {
			stbrp__findresult fr = stbrp__skyline_pack_rectangle(context, rects[i].w, rects[i].h);
			if (fr.prev_link) {
				rects[i].x = (stbrp_coord)fr.x;
				rects[i].y = (stbrp_coord)fr.y;
			}
			else {
				rects[i].x = rects[i].y = STBRP_MAXVAL;
			}
		}
	}
	qsort(rects, num_rects, sizeof(rects[0]), rect_original_order);
	for (i = 0; i < num_rects; ++i)
		rects[i].was_packed = !(rects[i].x == STBRP_MAXVAL && rects[i].y == STBRP_MAXVAL);
};

#endif
