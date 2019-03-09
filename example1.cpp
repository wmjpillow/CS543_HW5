//
//#include "Angel.h"
//
//typedef vec4 point4;
//typedef vec4 color4;
//
//GLuint mytex[1];
//GLuint buffers[2];
//GLuint loc, loc1;
//GLuint tex_loc;
//mat4 projection;
//GLuint projection_loc;
//
//point4  points[6] = {
//	point4(0.0, 0.0, 0.0, 1.0),
//	point4(0.0, 1.0, 0.0, 1.0),
//	point4(1.0, 1.0, 0.0, 1.0),
//	point4(1.0, 1.0, 0.0, 1.0),
//	point4(1.0, 0.0, 0.0, 1.0),
//	point4(0.0, 0.0, 0.0, 1.0)
//};
//
//GLfloat tex_coord[6][2] = { {0.0, 0.0}, {0.0, 1.0}, {1.0, 1.0}, {1.0, 1.0}, {1.0, 0.0}, {0.0, 0.0} };
//
//
///* default data*/
///* can enter other values via command line arguments */
//
//#define CENTERX -0.5
//#define CENTERY 0.5
//#define HEIGHT 0.5
//#define WIDTH 0.5
//#define MAX_ITER 100
//
///* N x M array to be generated */
//
//#define N 800
//#define M 600
//
//float height = HEIGHT; /* size of window in complex plane */
//float width = WIDTH;
//float cx = CENTERX; /* center of window in complex plane */
//float cy = CENTERY;
//int max = MAX_ITER; /* number of interations per point */
//
//int n = N;
//int m = M;
//
//
//GLfloat image[N][M][3];
//
///* complex data type and complex add, mult, and magnitude functions
//   probably not worth overhead */
//
//typedef float complex[2];
//
//void add(complex a, complex b, complex p)
//{
//	p[0] = a[0] + b[0];
//	p[1] = a[1] + b[1];
//}
//
//void mult(complex a, complex b, complex p)
//{
//	p[0] = a[0] * b[0] - a[1] * b[1];
//	p[1] = a[0] * b[1] + a[1] * b[0];
//}
//
//float mag2(complex a)
//{
//	return(a[0] * a[0] + a[1] * a[1]);
//}
//
//void form(float a, float b, complex p)
//{
//	p[0] = a;
//	p[1] = b;
//}
//
//void display()
//{
//	glClear(GL_COLOR_BUFFER_BIT);
//	glDrawArrays(GL_TRIANGLES, 0, 6);
//	glutSwapBuffers();
//}
//
//
//void myReshape(int w, int h)
//{
//	glViewport(0, 0, w, h);
//	projection = Ortho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
//	glUniformMatrix4fv(projection_loc, 16, GL_TRUE, projection);
//
//}
//
//void myinit()
//{
//	int i, j, k;
//	float x, y, v;
//	complex c0, c, d;
//
//	/* uncomment to specify your own parameters */
//
//	//    scanf("%f", &cx); /* center x */
//	//    scanf("%f", &cy); /* center y */
//	 //   scanf("%f", &width); /* rectangle width */
//	 //   height=width; /* rectangle height */
//	//    scanf("%d",&max); /* maximum iterations */
//
//	for (i = 0; i < N; i++) for (j = 0; j < M; j++)
//	{
//
//		/* starting point */
//
//		x = i * (width / (n - 1)) + cx - width / 2;
//		y = j * (height / (m - 1)) + cy - height / 2;
//
//		form(0, 0, c);
//		form(x, y, c0);
//
//		/* complex iteration */
//
//		for (k = 0; k < max; k++)
//		{
//			mult(c, c, d);
//			add(d, c0, c);
//			v = mag2(c);
//			if (v > 4.0) break; /* assume not in set if mag > 4 */
//		}
//
//
//		/* assign gray level to point based on its magnitude */
//
//		if (v > 1.0) v = 1.0; /* clamp if > 1 */
//		image[i][j][0] = v;
//		image[i][j][1] = rand() / ((float)RAND_MAX);
//		image[i][j][1] = 2.0*sin(v) - 1.0;
//		image[i][j][2] = 1.0 - v;
//	}
//
//	GLuint program = InitShader("vshader92.glsl", "fshader92.glsl");
//
//	glClearColor(1.0, 1.0, 1.0, 1.0);
//
//	glGenTextures(1, mytex);
//
//	glEnable(GL_TEXTURE_2D);
//
//	glBindTexture(GL_TEXTURE_2D, mytex[0]);
//	glTexImage2D(GL_TEXTURE_2D, 0, 3, n, m, 0, GL_RGB, GL_FLOAT, image);
//
//
//	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//
//
//	glActiveTexture(GL_TEXTURE1);
//	glBindTexture(GL_TEXTURE_2D, mytex[0]);
//
//
//	loc = glGetAttribLocation(program, "vPosition");
//	glEnableVertexAttribArray(loc);
//	glVertexAttribPointer(loc, 4, GL_FLOAT, GL_FALSE, 0, points);
//	loc1 = glGetAttribLocation(program, "texcoord");
//	glEnableVertexAttribArray(loc1);
//	glVertexAttribPointer(loc1, 2, GL_FLOAT, GL_FALSE, 0, tex_coord);
//	projection_loc = glGetUniformLocation(program, "projection");
//	tex_loc = glGetUniformLocation(program, "texMap");
//
//	/* set up vertex buffer object */
//
//	glGenBuffers(2, buffers);
//	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(points), NULL, GL_STATIC_DRAW);
//	glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(tex_coord), tex_coord, GL_STATIC_DRAW);
//
//	glUniform1i(tex_loc, mytex[0]);
//}
//
//
//int main(int argc, char *argv[])
//{
//	glutInit(&argc, argv);
//	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
//	glutInitWindowSize(N, M);
//	glutCreateWindow("mandlebrot");
//
//	glewInit();
//
//	myinit();
//
//	glutReshapeFunc(myReshape);
//	glutDisplayFunc(display);
//
//	glutMainLoop();
//	return 0;
//}

//#include "Angel.h"
//
//#include <complex>
//using std::complex;
//
//// Render the Mandelbrot set into the image array.
//// The parameters specify the region on the complex plane to plot.
//void compute_mandelbrot(double left, double right, double top, double bottom)
//{
//	// The number of times to iterate before we assume that a point isn't in the
//	// Mandelbrot set.
//	// (You may need to turn this up if you zoom further into the set.)
//	const int MAX_ITERATIONS = 500;
//
//	const int width = glutGet(GLUT_WINDOW_WIDTH);
//	const int height = glutGet(GLUT_WINDOW_HEIGHT);
//
//	glBegin(GL_POINTS); // start drawing in single pixel mode
//	for (int y = 0; y < height; ++y)
//	{
//		for (int x = 0; x < width; ++x)
//		{
//			// Work out the point in the complex plane that
//			// corresponds to this pixel in the output image.
//			complex<double> c(left + (x * (right - left) / width),
//				top + (y * (bottom - top) / height));
//
//			// Start off z at (0, 0).
//			complex<double> z(0.0, 0.0);
//
//			// Iterate z = z^2 + c until z moves more than 2 units
//			// away from (0, 0), or we've iterated too many times.
//			int iterations = 0;
//			while (abs(z) < 2.0 && iterations < MAX_ITERATIONS)
//			{
//				z = (z * z) + c;
//
//				++iterations;
//			}
//
//			if (iterations == MAX_ITERATIONS)
//			{
//				glColor3f(1.0, 0.0, 0.0); // Set color to draw mandelbrot
//				// z didn't escape from the circle.
//				// This point is in the Mandelbrot set.
//				glVertex2i(x, y);
//			}
//			else
//			{
//				glColor3f(0.0, 0.0, 0.0); //Set pixel to black
//				// z escaped within less than MAX_ITERATIONS
//				// iterations. This point isn't in the set.
//				glVertex2i(x, y);
//			}
//		}
//	}
//	glEnd();
//}
//
//void display_mandelbrot()
//{
//	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
//	glClear(GL_COLOR_BUFFER_BIT);
//
//	glMatrixMode(GL_PROJECTION);
//	glLoadIdentity();
//	const int width = glutGet(GLUT_WINDOW_WIDTH);
//	const int height = glutGet(GLUT_WINDOW_HEIGHT);
//	glOrtho(0, width, 0, height, -1, 1);
//
//	glMatrixMode(GL_MODELVIEW);
//	glLoadIdentity();
//
//	compute_mandelbrot(-2.0, 1.0, 1.125, -1.125);
//	glutSwapBuffers();
//}
//
//int main(int argc, char** argv)
//{
//	glutInit(&argc, argv);
//	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
//	glutInitWindowSize(800, 600);
//	glutCreateWindow("Mandelbrot");
//	glutDisplayFunc(display_mandelbrot);
//	glutMainLoop();
//	return 0;
//}
