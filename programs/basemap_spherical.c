#include <stdbool.h>
#include <GL/gl.h>

#include "../inlinebin.h"
#include "../programs.h"
#include "basemap_spherical.h"

enum	{ MAT_VIEWPROJ_INV
	, MAT_MODEL_INV
	, VERTEX
	} ;

static struct input inputs[] =
	{ [MAT_VIEWPROJ_INV] = { .name = "mat_viewproj_inv", .type = TYPE_UNIFORM }
	, [MAT_MODEL_INV]    = { .name = "mat_model_inv",    .type = TYPE_UNIFORM }
	, [VERTEX]           = { .name = "vertex",           .type = TYPE_ATTRIBUTE }
	,                      { .name = NULL }
	} ;

static struct program program =
	{ .name     = "basemap_spherical"
	, .vertex   = { .src = SHADER_BASEMAP_SPHERICAL_VERTEX }
	, .fragment = { .src = SHADER_BASEMAP_SPHERICAL_FRAGMENT }
	, .inputs   = inputs
	} ;

struct program *
program_basemap_spherical (void)
{
	return &program;
}

GLint
program_basemap_spherical_loc_vertex (void)
{
	return inputs[VERTEX].loc;
}

void
program_basemap_spherical_use (struct program_basemap_spherical *values)
{
	glUseProgram(program.id);
	glUniformMatrix4fv(inputs[MAT_VIEWPROJ_INV].loc, 1, GL_FALSE, values->mat_viewproj_inv);
	glUniformMatrix4fv(inputs[MAT_MODEL_INV].loc, 1, GL_FALSE, values->mat_model_inv);
}
