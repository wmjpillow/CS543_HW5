
#include "Angel.h"
#include "bmpread.h"
#include <complex>

#define  GLUT_WHEEL_UP 3           //mouse wheel 
#define  GLUT_WHEEL_DOWN 4

//////////
typedef Angel::vec4  color4;
typedef Angel::vec4  point4;
typedef Angel::vec3  point3;

using namespace std;

//global varible
GLuint program;
GLuint BmpProjection;
GLuint width = 0;
GLuint height = 0;
float  displayglobal = 85;
bmpread_t bitmap;
GLuint  textures[1];
int mode_flag;

// declare the routine
void keyboard( unsigned char key, int x, int y );
void initGPUAndShader( void );
void initpro( void ); //init state
void DrawPicture( void );
void displaybmp( void );
void runMandelbrot( void );

//----------------------------------------------------------------------------
void initGPUAndShader( void )
{
	//GPU init
    // Create a vertex array object
    GLuint vao;
    glGenVertexArrays( 1, &vao );
    glBindVertexArray( vao );

    // Create and initialize a buffer object
    GLuint buffer;
    glGenBuffers( 1, &buffer );
    glBindBuffer( GL_ARRAY_BUFFER, buffer );

	//Shader Init
	// Load shaders and use the resulting shader program
    program = InitShader( "vshader4bmp.glsl", "fshader4bmp.glsl" );
    glUseProgram( program );

	// sets the default color to clear screen
    glClearColor( 1.0, 1.0, 1.0, 1.0 ); // white background
}

// keyboard handler
void keyboard( unsigned char key, int x, int y )
{
    switch ( key ) {
	case 'O':
        mode_flag = 0;
        break;
	case 'L':
        mode_flag = 1;
        break;
	case 'N':
        mode_flag = 2;
        break;
	case 'D':
        mode_flag = 3;
        break;
	case 'E':
        mode_flag = 4;
        break;
	case 'T':
		mode_flag = 5;
		break;
	case 'W':
		mode_flag = 6;
		break;
	case 'P':
		mode_flag = 7;
		break;
	case 'S':
		mode_flag = 8;
		break;
	case 'R':
		runMandelbrot();
		break;
    case 033:
        exit( EXIT_SUCCESS );
        break;
	}
	glutPostRedisplay();
}

void initpro( void ){
	mode_flag = 0;

	printf("Loading usain_bolt.bmp\n");
	if(!bmpread("usain_bolt.bmp", 0, &bitmap))
	{
		fprintf(stderr, "%s:error loading bitmap file\n", "usain_bolt.bmp");
		exit(1);
	}

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}

void displaybmp( void )
{
	//glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	//glClearColor( 1.0, 1.0, 1.0, 1.0 );
	GLuint color_mode = glGetUniformLocationARB( program, "uColormode" ); 
	glUniform1i( color_mode, mode_flag );

    glClear( GL_COLOR_BUFFER_BIT );
    //glDrawArrays( GL_TRIANGLES, 0, 6 );
	glDrawArrays( GL_QUADS, 0, 4 );
    glutSwapBuffers();
}

void reshape( int width, int height )
{
    glViewport( 0, 0, width, height );
    //mat4 projection = Ortho( -1.0, 1.0, -1.0, 1.0, -1.0, 1.0 );
	Angel::mat4 projection = Angel::Perspective((GLfloat)displayglobal, (GLfloat)width/(GLfloat)height, (GLfloat)0.1, (GLfloat) 100.0);
    glUniformMatrix4fv( BmpProjection, 1, GL_TRUE, projection );
}

void DrawPicture( void ){
	Angel::mat4 modelMat = Angel::identity()* Angel::RotateX(0);//load 1,1,1,1 unit matrix
	GLuint modelMatrix = glGetUniformLocationARB(program, "model_matrix");
	glUniformMatrix4fv( modelMatrix, 1, GL_TRUE, modelMat );
	BmpProjection = glGetUniformLocation( program, "projection" );

	glGenTextures( 1, textures );
    glBindTexture( GL_TEXTURE_2D, textures[0] );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, bitmap.width, bitmap.height, 0, GL_RGB, GL_UNSIGNED_BYTE, bitmap.rgb_data );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	//bmpread_free(&bitmap);

	point4 points[4]={
		point4(-1.0,-1.0,0.0,1.0),
		point4(-1.0,1.0,0.0,1.0),
		point4(1.0,1.0,0.0,1.0),
		point4(1.0,-1.0,0.0,1.0),
	};
	point4 pointbmp[4]={
		point4(0.0,0.0,0.0,1.0),
		point4(0.0,1.0,0.0,1.0),
		point4(1.0,1.0,0.0,1.0),
		point4(1.0,0.0,0.0,1.0),
	};

    // Load shaders and use the resulting shader program
	glBufferData( GL_ARRAY_BUFFER, sizeof(points)*2, NULL, GL_STATIC_DRAW );
	glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(points), points );
	glBufferSubData( GL_ARRAY_BUFFER, sizeof(points), sizeof(pointbmp), pointbmp );
	GLuint vPosition = glGetAttribLocation( program, "vPosition" );
    glEnableVertexAttribArray( vPosition );
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
    //GLuint vColor = glGetAttribLocation( program, "vColor" ); 
    //glEnableVertexAttribArray( vColor );
    GLuint vTexCoord = glGetAttribLocation( program, "vTexCoord" ); 
    glEnableVertexAttribArray( vTexCoord );
    glVertexAttribPointer( vTexCoord, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(pointbmp)) );
    //glUniform1i( glGetUniformLocation(program, "uImageUnit"), 0 );

	glActiveTexture( GL_TEXTURE0 );
    glBindTexture(GL_TEXTURE_2D, textures[0]);

    glClearColor( 1.0, 1.0, 1.0, 1.0 );
}

//----------------------------------------------------------------------------
void compute_mandelbrot(double left, double right, double top, double bottom)
{
	// The number of times to iterate before we assume that a point isn't in the
	// Mandelbrot set.
	// (You may need to turn this up if you zoom further into the set.)
	const int MAX_ITERATIONS = 100;

	const int width = glutGet(GLUT_WINDOW_WIDTH);
	const int height = glutGet(GLUT_WINDOW_HEIGHT);

	glBegin(GL_POINTS); // start drawing in single pixel mode
	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			// Work out the point in the complex plane that
			// corresponds to this pixel in the output image.
			complex<double> c(left + (x * (right - left) / width),
				top + (y * (bottom - top) / height));

			// Start off z at (0, 0).
			complex<double> z(0.0, 0.0);

			// Iterate z = z^2 + c until z moves more than 2 units
			// away from (0, 0), or we've iterated too many times.
			int iterations = 0;
			while (abs(z) < 2.0 && iterations < MAX_ITERATIONS)
			{
				z = (z * z) + c;

				++iterations;
			}

			if (iterations == MAX_ITERATIONS)
			{
				glColor3f(1.0, 0.0, 0.0); // Set color to draw mandelbrot
				// z didn't escape from the circle.
				// This point is in the Mandelbrot set.
				glVertex2i(x, y);
			}
			else
			{
				glColor3f(0.0, 0.0, 0.0); //Set pixel to black
				// z escaped within less than MAX_ITERATIONS
				// iterations. This point isn't in the set.
				glVertex2i(x, y);
			}
		}
	}
	glEnd();
}

void display_mandelbrot()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	const int width = glutGet(GLUT_WINDOW_WIDTH);
	const int height = glutGet(GLUT_WINDOW_HEIGHT);
	glOrtho(0, width, 0, height, -1, 1);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	compute_mandelbrot(-2.0, 1.0, 1.125, -1.125);
	glutSwapBuffers();
}

void runMandelbrot()
{
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Mandelbrot");
	glutDisplayFunc(display_mandelbrot);
	glutMainLoop();
	return;
}


//----------------------------------------------------------------------------
// entry point
int main( int argc, char **argv )
{
	// init glut
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
	//glutInitDisplayMode( GLUT_RGBA | GLUT_SINGLE | GLUT_DEPTH ); //we don't use double buffer in hw3
	//glClear(GL_COLOR_BUFFER_BIT | DEPTH_BUFFER_BIT);
    glutInitWindowSize( 800, 600 );
	width = 800;
	height = 600;
    // If you are using freeglut, the next two lines will check if 
    // the code is truly 3.2. Otherwise, comment them out
    
    glutInitContextVersion( 3, 1 );
    glutInitContextProfile( GLUT_CORE_PROFILE );

	// create window
    glutCreateWindow( "Color Cube" );

	// init glew
    glewInit();

	initGPUAndShader();

	// assign handlers
	initpro();
	
	DrawPicture();
    glutDisplayFunc( displaybmp );
	glutReshapeFunc( reshape );
	//glutIdleFunc(idlehandle);
	//glutMouseFunc(mouse);
	//glutMotionFunc(processMouseActiveMotion);
    glutKeyboardFunc( keyboard );
	// should add menus

	// enter the drawing loop
    glutMainLoop();
    return 0;
}
