#include "figures_data.h"

std::deque<std::vector<GLfloat>> figures{
	std::vector<GLfloat>{
		// x       y      z      r     g     b
		  -0.95f, -0.95f, 0.0f,  1.0f, 0.0f, 0.0f,
		  -0.5f,  -0.2f,  0.0f,  0.0f, 1.0f, 0.0f,
		  -0.1f,  -0.95f, 0.0f,  0.0f, 0.0f, 1.0f
	},
	{
		// x       y      z      r     g     b
		   0.95f, -0.95f, 0.0f,  1.0f, 0.0f, 0.0f,
		   0.5f,  -0.2f,  0.0f,  0.0f, 1.0f, 0.0f,
		   0.1f,  -0.95f, 0.0f,  0.0f, 0.0f, 1.0f
	},
	{
		// x       y      z
		   0.1f, 0.1f, 0.0f,
		   0.2f, 0.4f, 0.0f,
		   0.6f, 0.7f, 0.0f,
		   0.8f, 0.7f, 0.0f,
		   0.8f, 0.4f, 0.0f,
		   0.3f, 0.1f, 0.0f
	},
	{
		// x       y      z
		   0.1f, 0.1f, 0.0f,
		   0.2f, 0.4f, 0.0f,
		   0.6f, 0.7f, 0.0f,
		   0.8f, 0.7f, 0.0f,
		   0.8f, 0.4f, 0.0f,
		   0.3f, 0.1f, 0.0f
	}
};

std::deque<std::vector<GLuint>> indices{
	{
		0, 1, 5,
		2, 3, 4,
		1, 2, 5,
		2, 4, 5
	},
	{
		0, 1, 5,
		2, 3, 4,
		1, 2, 5,
		2, 4, 5
	}
};