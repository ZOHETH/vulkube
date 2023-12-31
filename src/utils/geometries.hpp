struct VertexPC
{
	float x, y, z, w;   // Position
	float r, g, b, a;   // Color
};

struct VertexPT
{
	float x, y, z, w;   // Position data
	float u, v;         // texture u,v
};


static constexpr VertexPC coloredCubeData[] =
{
	// red face
	{ -1.0f, -1.0f,  1.0f, 1.0f,    1.0f, 0.0f, 0.0f, 1.0f },
	{ -1.0f,  1.0f,  1.0f, 1.0f,    1.0f, 0.0f, 0.0f, 1.0f },
	{  1.0f, -1.0f,  1.0f, 1.0f,    1.0f, 0.0f, 0.0f, 1.0f },
	{  1.0f, -1.0f,  1.0f, 1.0f,    1.0f, 0.0f, 0.0f, 1.0f },
	{ -1.0f,  1.0f,  1.0f, 1.0f,    1.0f, 0.0f, 0.0f, 1.0f },
	{  1.0f,  1.0f,  1.0f, 1.0f,    1.0f, 0.0f, 0.0f, 1.0f },
	// green face
	{ -1.0f, -1.0f, -1.0f, 1.0f,    0.0f, 1.0f, 0.0f, 1.0f },
	{  1.0f, -1.0f, -1.0f, 1.0f,    0.0f, 1.0f, 0.0f, 1.0f },
	{ -1.0f,  1.0f, -1.0f, 1.0f,    0.0f, 1.0f, 0.0f, 1.0f },
	{ -1.0f,  1.0f, -1.0f, 1.0f,    0.0f, 1.0f, 0.0f, 1.0f },
	{  1.0f, -1.0f, -1.0f, 1.0f,    0.0f, 1.0f, 0.0f, 1.0f },
	{  1.0f,  1.0f, -1.0f, 1.0f,    0.0f, 1.0f, 0.0f, 1.0f },
	// blue face
	{ -1.0f,  1.0f,  1.0f, 1.0f,    0.0f, 0.0f, 1.0f, 1.0f },
	{ -1.0f, -1.0f,  1.0f, 1.0f,    0.0f, 0.0f, 1.0f, 1.0f },
	{ -1.0f,  1.0f, -1.0f, 1.0f,    0.0f, 0.0f, 1.0f, 1.0f },
	{ -1.0f,  1.0f, -1.0f, 1.0f,    0.0f, 0.0f, 1.0f, 1.0f },
	{ -1.0f, -1.0f,  1.0f, 1.0f,    0.0f, 0.0f, 1.0f, 1.0f },
	{ -1.0f, -1.0f, -1.0f, 1.0f,    0.0f, 0.0f, 1.0f, 1.0f },
	// yellow face
	{  1.0f,  1.0f,  1.0f, 1.0f,    1.0f, 1.0f, 0.0f, 1.0f },
	{  1.0f,  1.0f, -1.0f, 1.0f,    1.0f, 1.0f, 0.0f, 1.0f },
	{  1.0f, -1.0f,  1.0f, 1.0f,    1.0f, 1.0f, 0.0f, 1.0f },
	{  1.0f, -1.0f,  1.0f, 1.0f,    1.0f, 1.0f, 0.0f, 1.0f },
	{  1.0f,  1.0f, -1.0f, 1.0f,    1.0f, 1.0f, 0.0f, 1.0f },
	{  1.0f, -1.0f, -1.0f, 1.0f,    1.0f, 1.0f, 0.0f, 1.0f },
	// magenta face
	{  1.0f,  1.0f,  1.0f, 1.0f,    1.0f, 0.0f, 1.0f, 1.0f },
	{ -1.0f,  1.0f,  1.0f, 1.0f,    1.0f, 0.0f, 1.0f, 1.0f },
	{  1.0f,  1.0f, -1.0f, 1.0f,    1.0f, 0.0f, 1.0f, 1.0f },
	{  1.0f,  1.0f, -1.0f, 1.0f,    1.0f, 0.0f, 1.0f, 1.0f },
	{ -1.0f,  1.0f,  1.0f, 1.0f,    1.0f, 0.0f, 1.0f, 1.0f },
	{ -1.0f,  1.0f, -1.0f, 1.0f,    1.0f, 0.0f, 1.0f, 1.0f },
	// cyan face
	{  1.0f, -1.0f,  1.0f, 1.0f,    0.0f, 1.0f, 1.0f, 1.0f },
	{  1.0f, -1.0f, -1.0f, 1.0f,    0.0f, 1.0f, 1.0f, 1.0f },
	{ -1.0f, -1.0f,  1.0f, 1.0f,    0.0f, 1.0f, 1.0f, 1.0f },
	{ -1.0f, -1.0f,  1.0f, 1.0f,    0.0f, 1.0f, 1.0f, 1.0f },
	{  1.0f, -1.0f, -1.0f, 1.0f,    0.0f, 1.0f, 1.0f, 1.0f },
	{ -1.0f, -1.0f, -1.0f, 1.0f,    0.0f, 1.0f, 1.0f, 1.0f },
};

static constexpr VertexPT texturedCubeData[] =
{
	// left face
	{ -1.0f, -1.0f, -1.0f, 1.0f,    1.0f, 0.0f },
	{ -1.0f,  1.0f,  1.0f, 1.0f,    0.0f, 1.0f },
	{ -1.0f, -1.0f,  1.0f, 1.0f,    0.0f, 0.0f },
	{ -1.0f,  1.0f,  1.0f, 1.0f,    0.0f, 1.0f },
	{ -1.0f, -1.0f, -1.0f, 1.0f,    1.0f, 0.0f },
	{ -1.0f,  1.0f, -1.0f, 1.0f,    1.0f, 1.0f },
	// front face
	{ -1.0f, -1.0f, -1.0f, 1.0f,    0.0f, 0.0f },
	{  1.0f, -1.0f, -1.0f, 1.0f,    1.0f, 0.0f },
	{  1.0f,  1.0f, -1.0f, 1.0f,    1.0f, 1.0f },
	{ -1.0f, -1.0f, -1.0f, 1.0f,    0.0f, 0.0f },
	{  1.0f,  1.0f, -1.0f, 1.0f,    1.0f, 1.0f },
	{ -1.0f,  1.0f, -1.0f, 1.0f,    0.0f, 1.0f },
	// top face
	{ -1.0f, -1.0f, -1.0f, 1.0f,    0.0f, 1.0f },
	{  1.0f, -1.0f,  1.0f, 1.0f,    1.0f, 0.0f },
	{  1.0f, -1.0f, -1.0f, 1.0f,    1.0f, 1.0f },
	{ -1.0f, -1.0f, -1.0f, 1.0f,    0.0f, 1.0f },
	{ -1.0f, -1.0f,  1.0f, 1.0f,    0.0f, 0.0f },
	{  1.0f, -1.0f, -1.0f, 1.0f,    1.0f, 0.0f },
	// bottom face
	{ -1.0f,  1.0f, -1.0f, 1.0f,    0.0f, 0.0f },
	{  1.0f,  1.0f,  1.0f, 1.0f,    1.0f, 1.0f },
	{ -1.0f,  1.0f,  1.0f, 1.0f,    0.0f, 1.0f },
	{ -1.0f,  1.0f, -1.0f, 1.0f,    0.0f, 0.0f },
	{  1.0f,  1.0f, -1.0f, 1.0f,    1.0f, 0.0f },
	{  1.0f,  1.0f,  1.0f, 1.0f,    1.0f, 1.0f },
	// right face
	{  1.0f,  1.0f, -1.0f, 1.0f,    0.0f, 1.0f },
	{  1.0f, -1.0f,  1.0f, 1.0f,    1.0f, 0.0f },
	{  1.0f,  1.0f,  1.0f, 1.0f,    1.0f, 1.0f },
	{  1.0f, -1.0f,  1.0f, 1.0f,    1.0f, 0.0f },
	{  1.0f,  1.0f, -1.0f, 1.0f,    0.0f, 1.0f },
	{  1.0f, -1.0f, -1.0f, 1.0f,    0.0f, 0.0f },
	// back face
	{ -1.0f,  1.0f,  1.0f, 1.0f,    1.0f, 1.0f },
	{  1.0f,  1.0f,  1.0f, 1.0f,    0.0f, 1.0f },
	{ -1.0f, -1.0f,  1.0f, 1.0f,    1.0f, 0.0f },
	{ -1.0f, -1.0f,  1.0f, 1.0f,    1.0f, 0.0f },
	{  1.0f,  1.0f,  1.0f, 1.0f,    0.0f, 1.0f },
	{  1.0f, -1.0f,  1.0f, 1.0f,    0.0f, 0.0f },
};
