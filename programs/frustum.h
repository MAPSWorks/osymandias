struct program_frustum {
	const float *mat_proj;
	const float *mat_frustum;
	float cx;
	float cy;
	int world_size;
	bool spherical;
};

struct program *program_frustum (void);
GLint program_frustum_loc_vertex (void);
void program_frustum_use (struct program_frustum *values);