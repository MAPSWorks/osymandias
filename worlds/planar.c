#include <stdbool.h>
#include <stdint.h>
#include <math.h>

#include "../matrix.h"
#include "../vector.h"
#include "../worlds.h"
#include "local.h"
#include "autoscroll.h"

// Transformation matrices:
static struct {
	float model[16];
} matrix;

static inline void
latlon_to_world (const struct world_state *state, float *x, float *y, const float lat, const float lon)
{
	*x = state->size * (0.5f + lon / (2.0f * M_PI));
	*y = state->size * (0.5f + atanhf(sinf(lat)) / (2.0f * M_PI));
}

static void
project (const struct world_state *state, float *vertex, float *normal, const float lat, const float lon)
{
	struct vector *v = (struct vector *)vertex;
	struct vector *n = (struct vector *)normal;

	// Convert lat/lon to world coordinates:
	latlon_to_world(state, &v->x, &v->y, lat, lon);

	v->z = 0.0f;
	v->w = 1.0f;

	n->x = 0.0f;
	n->y = 0.0f;
	n->z = 1.0f;
	n->w = 0.0f;

	// Apply model matrix:
	mat_vec_multiply(&v->x, matrix.model, &v->x);
	mat_vec_multiply(&n->x, matrix.model, &n->x);
}

static void
update_matrix_model (const struct world_state *state)
{
	float x, y;

	// Get world coordinates of current position:
	x = state->center.tile.x;
	y = state->size - state->center.tile.y;

	// Translate world so that this position is at origin:
	mat_translate(matrix.model, -x, -y, 0.0f);
}

static void
center_restrict_tile (struct world_state *state)
{
	// Clamp x and y coordinates to world size:
	clamp(state->center.tile.x, 0.0f, state->size);
	clamp(state->center.tile.y, 0.0f, state->size);
}

static void
center_restrict_latlon (struct world_state *state)
{
	clamp(state->center.lat, LAT_MIN, LAT_MAX);
	clamp(state->center.lon, LON_MIN, LON_MAX);
}

static void
move (const struct world_state *state)
{
	update_matrix_model(state);
}

static void
zoom (const struct world_state *state)
{
	update_matrix_model(state);
}

static const float *
matrix_model (void)
{
	return matrix.model;
}

static bool
timer_tick (struct world_state *state, int64_t usec)
{
	return autoscroll_update(state);
}

const struct world *
world_planar (void)
{
	static const struct world world = {
		.matrix			= matrix_model,
		.move			= move,
		.project		= project,
		.zoom			= zoom,
		.center_restrict_tile	= center_restrict_tile,
		.center_restrict_latlon	= center_restrict_latlon,
		.timer_tick		= timer_tick,
	};

	return &world;
}
