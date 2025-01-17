// OpenCL_Kernel.h
//
// MIT License
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWAR
//
// Oculus Rift : TM & Copyright Oculus VR, Inc. All Rights Reserved
// Unity : TM & Copyright Unity Technologies. All Rights Reserved
const char* kernel =
  "__constant sampler_t LINEAR = CLK_NORMALIZED_COORDS_FALSE | CLK_ADDRESS_CLAMP | CLK_FILTER_LINEAR;\n"
  "__constant sampler_t NEAREST = CLK_NORMALIZED_COORDS_FALSE | CLK_ADDRESS_CLAMP_TO_EDGE | CLK_FILTER_NEAREST;\n"
  "\n"
  "__kernel void remap(\n"
  "__read_only image2d_t src,			// CL_UNSIGNED_INT8 x 4\n"
  "__read_only image2d_t mapX,		// CL_FLOAT\n"
  "__read_only image2d_t mapY,		// CL_FLOAT\n"
  "__write_only image2d_t	dst)		// CL_UNSIGNED_INT8 x 4\n"
  "{\n"
  "	int x = get_global_id(0);\n"
  "	int y = get_global_id(1);\n"
  "	float X = read_imagef(mapX, (int2)(x, y)).x;\n"
  "	float Y = read_imagef(mapY, (int2)(x, y)).x;\n"
  "	uint4 pixel = read_imageui(src, LINEAR, (float2)(X, Y));\n"
  "	write_imageui(dst, (int2)(x, y), pixel);\n"
  "}\n"
  "\n"
  "__kernel void demosaic(\n"
  "		__read_only image2d_t src,		// CL_UNSIGNED_INT16\n"
  "		__write_only image2d_t left,	// CL_UNSIGNED_INT8 x 4\n"
  "		__write_only image2d_t right)	// CL_UNSIGNED_INT8 x 4\n"
  "{\n"
  "	// calculate 2 x 2 pixels block\n"
  "	int x = get_global_id(0) * 2;\n"
  "	int y = get_global_id(1) * 2;\n"
  "	uint patch[4][4];\n"
  "	uint4 rgb[2][2];\n"
  "	uint p[4][4];\n"
  "\n"
  "	// read data\n"
  "	patch[0][0] = read_imageui(src, NEAREST, (int2)(x - 1, y - 1)).x;\n"
  "	patch[0][1] = read_imageui(src, NEAREST, (int2)(x    , y - 1)).x;\n"
  "	patch[0][2] = read_imageui(src, NEAREST, (int2)(x + 1, y - 1)).x;\n"
  "	patch[0][3] = read_imageui(src, NEAREST, (int2)(x + 2, y - 1)).x;\n"
  "\n"
  "	patch[1][0] = read_imageui(src, NEAREST, (int2)(x - 1, y)).x;\n"
  "	patch[1][1] = read_imageui(src, NEAREST, (int2)(x    , y)).x;\n"
  "	patch[1][2] = read_imageui(src, NEAREST, (int2)(x + 1, y)).x;\n"
  "	patch[1][3] = read_imageui(src, NEAREST, (int2)(x + 2, y)).x;\n"
  "\n"
  "	patch[2][0] = read_imageui(src, NEAREST, (int2)(x - 1, y + 1)).x;\n"
  "	patch[2][1] = read_imageui(src, NEAREST, (int2)(x    , y + 1)).x;\n"
  "	patch[2][2] = read_imageui(src, NEAREST, (int2)(x + 1, y + 1)).x;\n"
  "	patch[2][3] = read_imageui(src, NEAREST, (int2)(x + 2, y + 1)).x;\n"
  "	\n"
  "	patch[3][0] = read_imageui(src, NEAREST, (int2)(x - 1, y + 2)).x;\n"
  "	patch[3][1] = read_imageui(src, NEAREST, (int2)(x    , y + 2)).x;\n"
  "	patch[3][2] = read_imageui(src, NEAREST, (int2)(x + 1, y + 2)).x;\n"
  "	patch[3][3] = read_imageui(src, NEAREST, (int2)(x + 2, y + 2)).x;\n"
  "\n"
  "	p[0][0] = patch[0][0] & 0xff;\n"
  "	p[0][1] = patch[0][1] & 0xff;\n"
  "	p[0][2] = patch[0][2] & 0xff;\n"
  "	p[0][3] = patch[0][3] & 0xff;\n"
  "	p[1][0] = patch[1][0] & 0xff;\n"
  "	p[1][1] = patch[1][1] & 0xff;\n"
  "	p[1][2] = patch[1][2] & 0xff;\n"
  "	p[1][3] = patch[1][3] & 0xff;\n"
  "	p[2][0] = patch[2][0] & 0xff;\n"
  "	p[2][1] = patch[2][1] & 0xff;\n"
  "	p[2][2] = patch[2][2] & 0xff;\n"
  "	p[2][3] = patch[2][3] & 0xff;\n"
  "	p[3][0] = patch[3][0] & 0xff;\n"
  "	p[3][1] = patch[3][1] & 0xff;\n"
  "	p[3][2] = patch[3][2] & 0xff;\n"
  "	p[3][3] = patch[3][3] & 0xff;\n"
  "\n"
  "	if (x == 0)\n"
  "	{\n"
  "		p[0][0] = p[0][2];\n"
  "		p[1][0] = p[1][2];\n"
  "		p[2][0] = p[2][2];\n"
  "		p[3][0] = p[3][2];\n"
  "	}\n"
  "	else if (x == get_image_width(src) - 2)\n"
  "	{\n"
  "		p[0][3] = p[0][1];\n"
  "		p[1][3] = p[1][1];\n"
  "		p[2][3] = p[2][1];\n"
  "		p[3][3] = p[3][1];\n"
  "	}\n"
  "\n"
  "\n"
  "	if (y == 0)\n"
  "	{\n"
  "		p[0][0] = p[2][0];\n"
  "		p[0][1] = p[2][1];\n"
  "		p[0][2] = p[2][2];\n"
  "		p[0][3] = p[2][3];\n"
  "	}\n"
  "	else if (y == get_image_height(src) - 2)\n"
  "	{\n"
  "		p[3][0] = p[1][0];\n"
  "		p[3][1] = p[1][1];\n"
  "		p[3][2] = p[1][2];\n"
  "		p[3][3] = p[1][3];\n"
  "	}\n"
  "\n"
  "	rgb[0][0].x = ((p[0][1] + p[2][1]) >> 1); // B \n"
  "	rgb[0][0].y = (p[1][1]); // G\n"
  "	rgb[0][0].z = ((p[1][0] + p[1][2]) >> 1); // R\n"
  "	rgb[0][0].w = 255;\n"
  "\n"
  "	rgb[0][1].x = (p[2][1]); // B\n"
  "	rgb[0][1].y = ((p[2][0] + p[2][2] + p[1][1] + p[3][1]) >> 2); // G\n"
  "	rgb[0][1].z = ((p[1][0] + p[3][0] + p[1][2] + p[3][2]) >> 2); // R\n"
  "	rgb[0][1].w = 255;\n"
  "\n"
  "	rgb[1][0].x = ((p[0][1] + p[2][1] + p[0][3] + p[2][3]) >> 2); // B\n"
  "	rgb[1][0].y = ((p[0][2] + p[2][2] + p[1][1] + p[1][3]) >> 2); // G\n"
  "	rgb[1][0].z = (p[1][2]); // R\n"
  "	rgb[1][0].w = 255;\n"
  "					\n"
  "	rgb[1][1].x = ((p[2][1] + p[2][3]) >> 1); // B \n"
  "	rgb[1][1].y = (p[2][2]); // G\n"
  "	rgb[1][1].z = ((p[1][2] + p[3][2]) >> 1); // R\n"
  "	rgb[1][1].w = 255;\n"
  "\n"
  "	// write pixels\n"
  "	write_imageui(left, (int2)(x, y), rgb[0][0]);\n"
  "	write_imageui(left, (int2)(x, y + 1), rgb[0][1]);\n"
  "	write_imageui(left, (int2)(x + 1, y), rgb[1][0]);\n"
  "	write_imageui(left, (int2)(x + 1, y + 1), rgb[1][1]);\n"
  "\n"
  "	p[0][0] = patch[0][0] >> 8;\n"
  "	p[0][1] = patch[0][1] >> 8;\n"
  "	p[0][2] = patch[0][2] >> 8;\n"
  "	p[0][3] = patch[0][3] >> 8;\n"
  "	p[1][0] = patch[1][0] >> 8;\n"
  "	p[1][1] = patch[1][1] >> 8;\n"
  "	p[1][2] = patch[1][2] >> 8;\n"
  "	p[1][3] = patch[1][3] >> 8;\n"
  "	p[2][0] = patch[2][0] >> 8;\n"
  "	p[2][1] = patch[2][1] >> 8;\n"
  "	p[2][2] = patch[2][2] >> 8;\n"
  "	p[2][3] = patch[2][3] >> 8;\n"
  "	p[3][0] = patch[3][0] >> 8;\n"
  "	p[3][1] = patch[3][1] >> 8;\n"
  "	p[3][2] = patch[3][2] >> 8;\n"
  "	p[3][3] = patch[3][3] >> 8;\n"
  "\n"
  "	if (x == 0)\n"
  "	{\n"
  "		p[0][0] = p[0][2];\n"
  "		p[1][0] = p[1][2];\n"
  "		p[2][0] = p[2][2];\n"
  "		p[3][0] = p[3][2];\n"
  "	}\n"
  "	else if (x == get_image_width(src) - 2)\n"
  "	{\n"
  "		p[0][3] = p[0][1];\n"
  "		p[1][3] = p[1][1];\n"
  "		p[2][3] = p[2][1];\n"
  "		p[3][3] = p[3][1];\n"
  "	}\n"
  "\n"
  "	if (y == 0)\n"
  "	{\n"
  "		p[0][0] = p[2][0];\n"
  "		p[0][1] = p[2][1];\n"
  "		p[0][2] = p[2][2];\n"
  "		p[0][3] = p[2][3];\n"
  "	}\n"
  "	else if (y == get_image_height(src) - 2)\n"
  "	{\n"
  "		p[3][0] = p[1][0];\n"
  "		p[3][1] = p[1][1];\n"
  "		p[3][2] = p[1][2];\n"
  "		p[3][3] = p[1][3];\n"
  "	}\n"
  "\n"
  "	rgb[0][0].x = ((p[0][1] + p[2][1]) >> 1); // B \n"
  "	rgb[0][0].y = (p[1][1]); // G\n"
  "	rgb[0][0].z = ((p[1][0] + p[1][2]) >> 1); // R\n"
  "	rgb[0][0].w = 255;\n"
  "\n"
  "	rgb[0][1].x = (p[2][1]); // B\n"
  "	rgb[0][1].y = ((p[2][0] + p[2][2] + p[1][1] + p[3][1]) >> 2); // G\n"
  "	rgb[0][1].z = ((p[1][0] + p[3][0] + p[1][2] + p[3][2]) >> 2); // R\n"
  "	rgb[0][1].w = 255;\n"
  "\n"
  "	rgb[1][0].x = ((p[0][1] + p[2][1] + p[0][3] + p[2][3]) >> 2); // B\n"
  "	rgb[1][0].y = ((p[0][2] + p[2][2] + p[1][1] + p[1][3]) >> 2); // G\n"
  "	rgb[1][0].z = (p[1][2]); // R\n"
  "	rgb[1][0].w = 255;\n"
  "					\n"
  "	rgb[1][1].x = ((p[2][1] + p[2][3]) >> 1); // B \n"
  "	rgb[1][1].y = (p[2][2]); // G\n"
  "	rgb[1][1].z = ((p[1][2] + p[3][2]) >> 1); // R\n"
  "	rgb[1][1].w = 255;\n"
  "\n"
  "	// write pixels\n"
  "	write_imageui(right, (int2)(x, y), rgb[0][0]);\n"
  "	write_imageui(right, (int2)(x, y + 1), rgb[0][1]);\n"
  "	write_imageui(right, (int2)(x + 1, y), rgb[1][0]);\n"
  "	write_imageui(right, (int2)(x + 1, y + 1), rgb[1][1]);\n"
  "}\n"
  "\n"
  "__kernel void resize( \n"
  "		__read_only image2d_t src,	// CL_UNSIGNED_INT8 x 4\n"
  "		__write_only image2d_t dst,	// CL_UNSIGNED_INT8 x 4\n"
  "		__read_only int scale)		// 2, 4, 8\n"
  "{\n"
  "	int x = get_global_id(0);\n"
  "	int y = get_global_id(1);\n"
  "	uint4 pixel = read_imageui(src, NEAREST, (int2)(x * scale, y * scale));\n"
  "	write_imageui(dst, (int2)(x, y), pixel);\n"
  "}\n"
  "\n"
  "__kernel void convertHSV( \n"
  "		__read_only image2d_t src,	// CL_UNSIGNED_INT8 x 4\n"
  "		__write_only image2d_t dst)	// CL_UNSIGNED_INT8 x 4\n"
  "{\n"
  "	int x = get_global_id(0);\n"
  "	int y = get_global_id(1);\n"
  "	uint4 pixel;\n"
  "	uint4 p[3][3];\n"
  "	p[0][0] = read_imageui(src, NEAREST, (int2)(x - 1, y - 1));\n"
  "	p[0][1] = read_imageui(src, NEAREST, (int2)(x - 1, y));\n"
  "	p[0][2] = read_imageui(src, NEAREST, (int2)(x - 1, y + 1));\n"
  "	p[1][0] = read_imageui(src, NEAREST, (int2)(x, y - 1));\n"
  "	p[1][1] = read_imageui(src, NEAREST, (int2)(x, y));\n"
  "	p[1][2] = read_imageui(src, NEAREST, (int2)(x, y + 1));\n"
  "	p[2][0] = read_imageui(src, NEAREST, (int2)(x + 1, y - 1));\n"
  "	p[2][1] = read_imageui(src, NEAREST, (int2)(x + 1, y));\n"
  "	p[2][2] = read_imageui(src, NEAREST, (int2)(x + 1, y + 1));\n"
  "	pixel.x = ((p[0][0].x + p[0][2].x + p[2][0].x + p[2][2].x)"
  "			+ (p[0][1].x + p[1][0].x + p[1][2].x + p[2][1].x) * 2 "
  "			+ p[1][1].x * 4) / 16;\n"
  "	pixel.y = ((p[0][0].y + p[0][2].y + p[2][0].y + p[2][2].y)"
  "			+ (p[0][1].y + p[1][0].y + p[1][2].y + p[2][1].y) * 2 "
  "			+ p[1][1].y * 4) / 16;\n"
  "	pixel.z = ((p[0][0].z + p[0][2].z + p[2][0].z + p[2][2].z)"
  "			+ (p[0][1].z + p[1][0].z + p[1][2].z + p[2][1].z) * 2 "
  "			+ p[1][1].z * 4) / 16;\n"
  "	uint4 hsv = (uint4)(0, 0, 0, 255);\n"
  "	int hue;\n"
  "	uint v = max(max(pixel.x, pixel.y), pixel.z);\n"
  "	int delta = v - min(min(pixel.x, pixel.y), pixel.z);\n"
  "	if (v != 0) {\n"
  "		hsv.z = v;\n"
  "		hsv.y = (delta * 255) / v;\n"
  "		if (v == pixel.z) {\n"
  "			hue = (60 * ((int)pixel.y - (int)pixel.x)) / delta;\n"
  "		} else if (v == pixel.y) {\n"
  "			hue = (60 * ((int)pixel.x - (int)pixel.z)) / delta + 120;\n"
  "		} else {\n"
  "			hue = (60 * ((int)pixel.z - (int)pixel.y)) / delta + 240;\n"
  "		}\n"
  "		if (hue < 0) {\n"
  "			hsv.x = (uint)(hue + 360) / 2;\n"
  "		} else {\n"
  "			hsv.x = (uint)hue / 2;\n"
  "		}\n"
  "	}\n"
  "	write_imageui(dst, (int2)(x, y), hsv);\n"
  "}\n"
  "\n"
  "__kernel void convertGrayscale( \n"
  "		__read_only image2d_t src,	// CL_UNSIGNED_INT8 x 4\n"
  "		__write_only image2d_t dst)	// CL_UNSIGNED_INT8\n"
  "{\n"
  "	int x = get_global_id(0);\n"
  "	int y = get_global_id(1);\n"
  "	uint4 pixel = read_imageui(src, NEAREST, (int2)(x, y));\n"
  "	uint4 gray;\n"
  "	gray.x = ((299 * pixel.z) + (587 * pixel.y) + (114 * pixel.x)) / 1000;\n"
  "	write_imageui(dst, (int2)(x, y), gray);\n"
  "}\n"
  "\n"
  "__kernel void gaussian3x3( \n"
  "		__read_only image2d_t src,	// CL_UNSIGNED_INT8 x 4\n"
  "		__write_only image2d_t dst)	// CL_UNSIGNED_INT8 x 4\n"
  "{\n"
  "	int x = get_global_id(0);\n"
  "	int y = get_global_id(1);\n"
  "	uint4 pixel;\n"
  "	uint4 p[3][3];\n"
  "	p[0][0] = read_imageui(src, NEAREST, (int2)(x - 1, y - 1));\n"
  "	p[0][1] = read_imageui(src, NEAREST, (int2)(x - 1, y));\n"
  "	p[0][2] = read_imageui(src, NEAREST, (int2)(x - 1, y + 1));\n"
  "	p[1][0] = read_imageui(src, NEAREST, (int2)(x, y - 1));\n"
  "	p[1][1] = read_imageui(src, NEAREST, (int2)(x, y));\n"
  "	p[1][2] = read_imageui(src, NEAREST, (int2)(x, y + 1));\n"
  "	p[2][0] = read_imageui(src, NEAREST, (int2)(x + 1, y - 1));\n"
  "	p[2][1] = read_imageui(src, NEAREST, (int2)(x + 1, y));\n"
  "	p[2][2] = read_imageui(src, NEAREST, (int2)(x + 1, y + 1));\n"
  "	pixel.x = ((p[0][0].x + p[0][2].x + p[2][0].x + p[2][2].x)"
  "			+ (p[0][1].x + p[1][0].x + p[1][2].x + p[2][1].x) * 2 "
  "			+ p[1][1].x * 4) / 16;\n"
  "	pixel.y = ((p[0][0].y + p[0][2].y + p[2][0].y + p[2][2].y)"
  "			+ (p[0][1].y + p[1][0].y + p[1][2].y + p[2][1].y) * 2 "
  "			+ p[1][1].y * 4) / 16;\n"
  "	pixel.z = ((p[0][0].z + p[0][2].z + p[2][0].z + p[2][2].z)"
  "			+ (p[0][1].z + p[1][0].z + p[1][2].z + p[2][1].z) * 2 "
  "			+ p[1][1].z * 4) / 16;\n"
  "	pixel.w = 255;\n"
  "	write_imageui(dst, (int2)(x, y), pixel);\n"
  "}\n"
  "\n"
  "__kernel void median3x3_H( \n"
  "		__read_only image2d_t src,	// CL_UNSIGNED_INT8 x 4\n"
  "		__write_only image2d_t dst)	// CL_UNSIGNED_INT8 x 4\n"
  "{\n"
  "	int x = get_global_id(0);\n"
  "	int y = get_global_id(1);\n"
  "	uint i, j, k;"
  "	uint4 p[9], q[5], min;\n"
  "	p[0] = read_imageui(src, NEAREST, (int2)(x - 1, y - 1));\n"
  "	p[1] = read_imageui(src, NEAREST, (int2)(x - 1, y));\n"
  "	p[2] = read_imageui(src, NEAREST, (int2)(x - 1, y + 1));\n"
  "	p[3] = read_imageui(src, NEAREST, (int2)(x, y - 1));\n"
  "	p[4] = read_imageui(src, NEAREST, (int2)(x, y));\n"
  "	p[5] = read_imageui(src, NEAREST, (int2)(x, y + 1));\n"
  "	p[6] = read_imageui(src, NEAREST, (int2)(x + 1, y - 1));\n"
  "	p[7] = read_imageui(src, NEAREST, (int2)(x + 1, y));\n"
  "	p[8] = read_imageui(src, NEAREST, (int2)(x + 1, y + 1));\n"
  "	for (i = 0; i < 5; i++) {\n"
  "		min = (uint4)(255, 0, 0, 0); k = 0;\n"
  "		for (j = 0; j < 9; j++) {\n"
  "			if (p[j].x < min.x) {\n"
  "				min = p[j]; k = j;\n"
  "			}\n"
  "		}\n"
  "		q[i] = min; p[k].x = 255;\n"
  "	}\n"
  "	write_imageui(dst, (int2)(x, y), q[4]);\n"
  "}\n"
  "\n"
  "__kernel void median5x5_H( \n"
  "		__read_only image2d_t src,	// CL_UNSIGNED_INT8 x 4\n"
  "		__write_only image2d_t dst)	// CL_UNSIGNED_INT8 x 4\n"
  "{\n"
  "	int x = get_global_id(0);\n"
  "	int y = get_global_id(1);\n"
  "	uint i, j, k;"
  "	uint4 p[25], q[13], min;\n"
  "	p[0] = read_imageui(src, NEAREST, (int2)(x - 1, y - 1));\n"
  "	p[1] = read_imageui(src, NEAREST, (int2)(x - 1, y));\n"
  "	p[2] = read_imageui(src, NEAREST, (int2)(x - 1, y + 1));\n"
  "	p[3] = read_imageui(src, NEAREST, (int2)(x, y - 1));\n"
  "	p[4] = read_imageui(src, NEAREST, (int2)(x, y));\n"
  "	p[5] = read_imageui(src, NEAREST, (int2)(x, y + 1));\n"
  "	p[6] = read_imageui(src, NEAREST, (int2)(x + 1, y - 1));\n"
  "	p[7] = read_imageui(src, NEAREST, (int2)(x + 1, y));\n"
  "	p[8] = read_imageui(src, NEAREST, (int2)(x + 1, y + 1));\n"
  "	p[9] = read_imageui(src, NEAREST, (int2)(x - 2, y - 2));\n"
  "	p[10] = read_imageui(src, NEAREST, (int2)(x - 2, y - 1));\n"
  "	p[11] = read_imageui(src, NEAREST, (int2)(x - 2, y));\n"
  "	p[12] = read_imageui(src, NEAREST, (int2)(x - 2, y + 1));\n"
  "	p[13] = read_imageui(src, NEAREST, (int2)(x - 2, y + 2));\n"
  "	p[14] = read_imageui(src, NEAREST, (int2)(x + 2, y - 2));\n"
  "	p[15] = read_imageui(src, NEAREST, (int2)(x + 2, y - 1));\n"
  "	p[16] = read_imageui(src, NEAREST, (int2)(x + 2, y));\n"
  "	p[17] = read_imageui(src, NEAREST, (int2)(x + 2, y + 1));\n"
  "	p[18] = read_imageui(src, NEAREST, (int2)(x + 2, y + 2));\n"
  "	p[19] = read_imageui(src, NEAREST, (int2)(x - 1, y - 2));\n"
  "	p[20] = read_imageui(src, NEAREST, (int2)(x, y - 2));\n"
  "	p[21] = read_imageui(src, NEAREST, (int2)(x + 1, y - 2));\n"
  "	p[22] = read_imageui(src, NEAREST, (int2)(x - 1, y + 2));\n"
  "	p[23] = read_imageui(src, NEAREST, (int2)(x, y + 2));\n"
  "	p[24] = read_imageui(src, NEAREST, (int2)(x + 1, y + 2));\n"
  "\n"
  "	for (i = 0; i < 13; i++) {\n"
  "		min = (uint4)(255, 0, 0, 0); k = 0;\n"
  "		for (j = 0; j < 25; j++) {\n"
  "			if (p[j].x < min.x) {\n"
  "				min = p[j]; k = j;\n"
  "			}\n"
  "		}\n"
  "		q[i] = min; p[k].x = 255;\n"
  "	}\n"
  "	write_imageui(dst, (int2)(x, y), q[12]);\n"
  "}\n"
  "\n"
  "__kernel void skincolor( \n"
  "		__read_only image2d_t src,	// CL_UNSIGNED_INT8 x 4\n"
  "		__write_only image2d_t mask,// CL_UNSIGNED_INT8\n"
  "		__read_only int h_low,		// \n"
  "		__read_only int h_hight,	// \n"
  "		__read_only int s_low,		// \n"
  "		__read_only int s_hight)	// \n"
  "{\n"
  "	int x = get_global_id(0);\n"
  "	int y = get_global_id(1);\n"
  "	uint4 set = (uint4)(255, 0, 0, 0);\n"
  "	uint4 clear = (uint4)(0, 0, 0, 0);\n"
  "	uint4 hsv = read_imageui(src, NEAREST, (int2)(x, y));\n"
  "	if (h_low <= hsv.x && hsv.x <= h_hight && s_low <= hsv.y && hsv.y <= s_hight) {\n"
  "		write_imageui(mask, (int2)(x, y), set);\n"
  "	} else {\n"
  "		write_imageui(mask, (int2)(x, y), clear);\n"
  "	}\n"
  "}\n"
  "\n"
  "__kernel void invert_mask( \n"
  "		__read_only image2d_t src,	// CL_UNSIGNED_INT8 x 4\n"
  "		__write_only image2d_t dst,	// CL_UNSIGNED_INT8 x 4\n"
  "		__read_only image2d_t mask,	// CL_UNSIGNED_INT8\n"
  "		__read_only int threshold)	// \n"
  "{\n"
  "	int x = get_global_id(0);\n"
  "	int y = get_global_id(1);\n"
  "	uint4 clear = (uint4)(0, 0, 0, 255);\n"
  "	uint4 pixel = read_imageui(src, NEAREST, (int2)(x, y));\n"
  "	uint4 m = read_imageui(mask, NEAREST, (int2)(x, y));\n"
  "	pixel.w = 255 - m.x;\n"
  "	if (threshold < pixel.w) write_imageui(dst, (int2)(x, y), pixel);\n"
  "	else write_imageui(dst, (int2)(x, y), clear);\n"
  "}\n"
  "\n"
  "__kernel void mask( \n"
  "		__read_only image2d_t src,	// CL_UNSIGNED_INT8 x 4\n"
  "		__write_only image2d_t dst,	// CL_UNSIGNED_INT8 x 4\n"
  "		__read_only image2d_t mask,	// CL_UNSIGNED_INT8\n"
  "		__read_only int threshold)	// \n"
  "{\n"
  "	int x = get_global_id(0);\n"
  "	int y = get_global_id(1);\n"
  "	uint4 clear = (uint4)(0, 0, 0, 255);\n"
  "	uint4 pixel = read_imageui(src, NEAREST, (int2)(x, y));\n"
  "	uint4 m = read_imageui(mask, NEAREST, (int2)(x, y));\n"
  "	pixel.w = m.x;\n"
  "	if (threshold < m.x) write_imageui(dst, (int2)(x, y), pixel);\n"
  "	else write_imageui(dst, (int2)(x, y), clear);\n"
  "}\n"
  "\n"
  "__kernel void mask_opengl( \n"
  "		__read_only image2d_t src,	// CL_UNSIGNED_INT8 x 4\n"
  "		__write_only image2d_t dst,	// CL_UNORM_INT8 x 4\n"
  "		__read_only image2d_t mask,	// CL_UNSIGNED_INT8\n"
  "		__read_only int threshold)	// \n"
  "{\n"
  "	int x = get_global_id(0);\n"
  "	int y = get_global_id(1);\n"
  "	float4 clear = (float4)(0.0f, 0.0f, 0.0f, 1.0f);\n"
  "	uint4 pixel = read_imageui(src, NEAREST, (int2)(x, y));\n"
  "	uint4 m = read_imageui(mask, NEAREST, (int2)(x, y));\n"
  "	pixel.w = m.x;\n"
  "	float4 f_pixel = (float4)(pixel.z / 255.0f, pixel.y / 255.0f, pixel.x / 255.0f, pixel.w / 255.0f);\n"
  "	if (threshold < m.x) write_imagef(dst, (int2)(x, y), f_pixel);\n"
  "	else write_imagef(dst, (int2)(x, y), clear);\n"
  "}\n"
  "\n"
  "__kernel void mask_d3d11( \n"
  "		__read_only image2d_t src,	// CL_UNSIGNED_INT8 x 4\n"
  "		__write_only image2d_t dst,	// CL_UNORM_INT8 x 4\n"
  "		__read_only image2d_t mask,	// CL_UNSIGNED_INT8\n"
  "		__read_only int threshold)	// \n"
  "{\n"
  "	int x = get_global_id(0);\n"
  "	int y = get_global_id(1);\n"
  "	float4 clear = (float4)(0.0f, 0.0f, 0.0f, 1.0f);\n"
  "	uint4 pixel = read_imageui(src, NEAREST, (int2)(x, y));\n"
  "	uint4 m = read_imageui(mask, NEAREST, (int2)(x, y));\n"
  "	pixel.w = m.x;\n"
  "	float4 f_pixel = (float4)(pixel.x / 255.0f, pixel.y / 255.0f, pixel.y / 255.0f, pixel.w / 255.0f);\n"
  "	if (threshold < m.x) write_imagef(dst, (int2)(x, y), f_pixel);\n"
  "	else write_imagef(dst, (int2)(x, y), clear);\n"
  "}\n"
  "\n"
  "__kernel void gamma( \n"
  "		__read_only image2d_t src,	// CL_UNSIGNED_INT8 x 4\n"
  "		__write_only image2d_t dst,	// CL_UNSIGNED_INT8 x 4\n"
  "		__read_only float gammaCorrection)	// \n"
  "{\n"
  "	int x = get_global_id(0);\n"
  "	int y = get_global_id(1);\n"
  "	uint4 pixel = read_imageui(src, NEAREST, (int2)(x, y));\n"
  "	pixel.x = (uint)(255 * powr(pixel.x / 255.0f, gammaCorrection));\n"
  "	pixel.y = (uint)(255 * powr(pixel.y / 255.0f, gammaCorrection));\n"
  "	pixel.z = (uint)(255 * powr(pixel.z / 255.0f, gammaCorrection));\n"
  "	write_imageui(dst, (int2)(x, y), pixel);\n"
  "}\n"
  "\n"
  "__kernel void tone(\n"
  "		__read_only image2d_t src,			// CL_UNSIGNED_INT8 x 4\n"
  "		__write_only image2d_t	dst,		// CL_UNSIGNED_INT8 x 4\n"
  "		__read_only image2d_t map)			// CL_UNSIGNED_INT8\n"
  "{\n"
  "	int x = get_global_id(0);\n"
  "	int y = get_global_id(1);\n"
  "	uint4 pixel = read_imageui(src, NEAREST, (int2)(x, y));\n"
  "	uint4 xval = read_imageui(map, (int2)(pixel.x, 0));\n"
  "	uint4 yval = read_imageui(map, (int2)(pixel.y, 1));\n"
  "	uint4 zval = read_imageui(map, (int2)(pixel.z, 2));\n"
  "	write_imageui(dst, (int2)(x, y), (uint4)(xval.x, yval.x, zval.x, pixel.w));\n"
  "}\n"
  "\n"
  "__kernel void resize_tone( \n"
  "		__read_only image2d_t src,	// CL_UNSIGNED_INT8 x 4\n"
  "		__write_only image2d_t dst,	// CL_UNSIGNED_INT8 x 4\n"
  "		__read_only int scale,		// 2, 4, 8\n"
  "		__read_only image2d_t map)	// CL_UNSIGNED_INT8\n"
  "{\n"
  "	int x = get_global_id(0);\n"
  "	int y = get_global_id(1);\n"
  "	uint4 pixel = read_imageui(src, NEAREST, (int2)(x * scale, y * scale));\n"
  "	uint4 xval = read_imageui(map, (int2)(pixel.x, 0));\n"
  "	uint4 yval = read_imageui(map, (int2)(pixel.y, 1));\n"
  "	uint4 zval = read_imageui(map, (int2)(pixel.z, 2));\n"
  "	write_imageui(dst, (int2)(x, y), (uint4)(xval.x, yval.x, zval.x, pixel.w));\n"
  "}\n"
  "\n"
  "__kernel void convertHSVTone( \n"
  "		__read_only image2d_t src,	// CL_UNSIGNED_INT8 x 4\n"
  "		__write_only image2d_t dst,	// CL_UNSIGNED_INT8 x 4\n"
  "		__read_only image2d_t map)	// CL_UNSIGNED_INT8\n"
  "{\n"
  "	int x = get_global_id(0);\n"
  "	int y = get_global_id(1);\n"
  "	uint4 pixel;\n"
  "	uint4 p[3][3];\n"
  "	p[0][0] = read_imageui(src, NEAREST, (int2)(x - 1, y - 1));\n"
  "	p[0][1] = read_imageui(src, NEAREST, (int2)(x - 1, y));\n"
  "	p[0][2] = read_imageui(src, NEAREST, (int2)(x - 1, y + 1));\n"
  "	p[1][0] = read_imageui(src, NEAREST, (int2)(x, y - 1));\n"
  "	p[1][1] = read_imageui(src, NEAREST, (int2)(x, y));\n"
  "	p[1][2] = read_imageui(src, NEAREST, (int2)(x, y + 1));\n"
  "	p[2][0] = read_imageui(src, NEAREST, (int2)(x + 1, y - 1));\n"
  "	p[2][1] = read_imageui(src, NEAREST, (int2)(x + 1, y));\n"
  "	p[2][2] = read_imageui(src, NEAREST, (int2)(x + 1, y + 1));\n"
  "	pixel.x = ((p[0][0].x + p[0][2].x + p[2][0].x + p[2][2].x)"
  "			+ (p[0][1].x + p[1][0].x + p[1][2].x + p[2][1].x) * 2 "
  "			+ p[1][1].x * 4) / 16;\n"
  "	pixel.y = ((p[0][0].y + p[0][2].y + p[2][0].y + p[2][2].y)"
  "			+ (p[0][1].y + p[1][0].y + p[1][2].y + p[2][1].y) * 2 "
  "			+ p[1][1].y * 4) / 16;\n"
  "	pixel.z = ((p[0][0].z + p[0][2].z + p[2][0].z + p[2][2].z)"
  "			+ (p[0][1].z + p[1][0].z + p[1][2].z + p[2][1].z) * 2 "
  "			+ p[1][1].z * 4) / 16;\n"
  "	uint4 xval = read_imageui(map, (int2)(pixel.x, 0));\n"
  "	uint4 yval = read_imageui(map, (int2)(pixel.y, 1));\n"
  "	uint4 zval = read_imageui(map, (int2)(pixel.z, 2));\n"
  "	pixel = (uint4)(xval.x, yval.x, zval.x, pixel.w);\n"
  "	uint4 hsv = (uint4)(0, 0, 0, 255);\n"
  "	int hue;\n"
  "	uint v = max(max(pixel.x, pixel.y), pixel.z);\n"
  "	int delta = v - min(min(pixel.x, pixel.y), pixel.z);\n"
  "	if (v != 0) {\n"
  "		hsv.z = v;\n"
  "		hsv.y = (delta * 255) / v;\n"
  "		if (v == pixel.z) {\n"
  "			hue = (60 * ((int)pixel.y - (int)pixel.x)) / delta;\n"
  "		} else if (v == pixel.y) {\n"
  "			hue = (60 * ((int)pixel.x - (int)pixel.z)) / delta + 120;\n"
  "		} else {\n"
  "			hue = (60 * ((int)pixel.z - (int)pixel.y)) / delta + 240;\n"
  "		}\n"
  "		if (hue < 0) {\n"
  "			hsv.x = (uint)(hue + 360) / 2;\n"
  "		} else {\n"
  "			hsv.x = (uint)hue / 2;\n"
  "		}\n"
  "	}\n"
  "	write_imageui(dst, (int2)(x, y), hsv);\n"
  "}\n"
  "\n"
  "__kernel void maskTone( \n"
  "		__read_only image2d_t src,	// CL_UNSIGNED_INT8 x 4\n"
  "		__write_only image2d_t dst,	// CL_UNSIGNED_INT8 x 4\n"
  "		__read_only image2d_t mask,	// CL_UNSIGNED_INT8\n"
  "		__read_only int threshold,	// \n"
  "		__read_only image2d_t map)	// CL_UNSIGNED_INT8\n"
  "{\n"
  "	int x = get_global_id(0);\n"
  "	int y = get_global_id(1);\n"
  "	uint4 clear = (uint4)(0, 0, 0, 255);\n"
  "	uint4 pixel = read_imageui(src, NEAREST, (int2)(x, y));\n"
  "	uint4 m = read_imageui(mask, NEAREST, (int2)(x, y));\n"
  "	pixel.w = m.x;\n"
  "	uint4 xval = read_imageui(map, (int2)(pixel.x, 0));\n"
  "	uint4 yval = read_imageui(map, (int2)(pixel.y, 1));\n"
  "	uint4 zval = read_imageui(map, (int2)(pixel.z, 2));\n"
  "	pixel = (uint4)(xval.x, yval.x, zval.x, pixel.w);\n"
  "	if (threshold < m.x) write_imageui(dst, (int2)(x, y), pixel);\n"
  "	else write_imageui(dst, (int2)(x, y), clear);\n"
  "}\n"
  "\n"
  "__kernel void mask_openglTone( \n"
  "		__read_only image2d_t src,	// CL_UNSIGNED_INT8 x 4\n"
  "		__write_only image2d_t dst,	// CL_UNORM_INT8 x 4\n"
  "		__read_only image2d_t mask,	// CL_UNSIGNED_INT8\n"
  "		__read_only int threshold,	// \n"
  "		__read_only image2d_t map)	// CL_UNSIGNED_INT8\n"
  "{\n"
  "	int x = get_global_id(0);\n"
  "	int y = get_global_id(1);\n"
  "	float4 clear = (float4)(0.0f, 0.0f, 0.0f, 1.0f);\n"
  "	uint4 pixel = read_imageui(src, NEAREST, (int2)(x, y));\n"
  "	uint4 xval = read_imageui(map, (int2)(pixel.x, 0));\n"
  "	uint4 yval = read_imageui(map, (int2)(pixel.y, 1));\n"
  "	uint4 zval = read_imageui(map, (int2)(pixel.z, 2));\n"
  "	pixel = (uint4)(xval.x, yval.x, zval.x, pixel.w);\n"
  "	uint4 m = read_imageui(mask, NEAREST, (int2)(x, y));\n"
  "	pixel.w = m.x;\n"
  "	float4 f_pixel = (float4)(pixel.z / 255.0f, pixel.y / 255.0f, pixel.x / 255.0f, pixel.w / 255.0f);\n"
  "	if (threshold < m.x) write_imagef(dst, (int2)(x, y), f_pixel);\n"
  "	else write_imagef(dst, (int2)(x, y), clear);\n"
  "}\n"
  "\n"
  "__kernel void mask_d3d11Tone( \n"
  "		__read_only image2d_t src,	// CL_UNSIGNED_INT8 x 4\n"
  "		__write_only image2d_t dst,	// CL_UNORM_INT8 x 4\n"
  "		__read_only image2d_t mask,	// CL_UNSIGNED_INT8\n"
  "		__read_only int threshold,	// \n"
  "		__read_only image2d_t map)	// CL_UNSIGNED_INT8\n"
  "{\n"
  "	int x = get_global_id(0);\n"
  "	int y = get_global_id(1);\n"
  "	float4 clear = (float4)(0.0f, 0.0f, 0.0f, 1.0f);\n"
  "	uint4 pixel = read_imageui(src, NEAREST, (int2)(x, y));\n"
  "	uint4 xval = read_imageui(map, (int2)(pixel.x, 0));\n"
  "	uint4 yval = read_imageui(map, (int2)(pixel.y, 1));\n"
  "	uint4 zval = read_imageui(map, (int2)(pixel.z, 2));\n"
  "	pixel = (uint4)(xval.x, yval.x, zval.x, pixel.w);\n"
  "	uint4 m = read_imageui(mask, NEAREST, (int2)(x, y));\n"
  "	pixel.w = m.x;\n"
  "	float4 f_pixel = (float4)(pixel.x / 255.0f, pixel.y / 255.0f, pixel.y / 255.0f, pixel.w / 255.0f);\n"
  "	if (threshold < m.x) write_imagef(dst, (int2)(x, y), f_pixel);\n"
  "	else write_imagef(dst, (int2)(x, y), clear);\n"
  "}\n"
  "\n"
  "__kernel void copy_opengl( \n"
  "		__read_only image2d_t src,	// CL_UNSIGNED_INT8 x 4\n"
  "		__write_only image2d_t dst)	// CL_UNORM_INT8 x 4\n"
  "{\n"
  "	int x = get_global_id(0);\n"
  "	int y = get_global_id(1);\n"
  "	uint4 pixel = read_imageui(src, NEAREST, (int2)(x, y));\n"
  "	float4 f_pixel = (float4)(pixel.z / 255.0f, pixel.y / 255.0f, pixel.x / 255.0f, pixel.w / 255.0f);\n"
  "	write_imagef(dst, (int2)(x, y), f_pixel);\n"
  "}\n"
  "\n"
  ;