#ifndef RECT_H
#define RECT_H

#include <gl/gl.h>

#define BETWEEN(n, a, b) (n >= a && n <= b)

typedef struct GL_RECT
{
	LONG x_L;
	LONG y_T;
	LONG x_R;
	LONG y_B;
} GL_RECT;

/* Converting client coordinates to world coordinates */
double CLX_TO_WRX(LONG x, LONG screen_width)
{
	return (-1.0 + (2.0 * x) / screen_width);
}

double CLY_TO_WRY(LONG y, LONG screen_height)
{
	return (-1.0 + (2.0 * y) / screen_height);
}

// Set the coordinates of a rectangle
void setRect(GL_RECT *rect, LONG x_L, LONG y_T, LONG x_R, LONG y_B)
{
	(*rect).x_L = x_L;
	(*rect).y_T = y_T;
	(*rect).x_R = x_R;
	(*rect).y_B = y_B;
}

// Move a rectangle by a specific number of pixels within the bounds of a window
void moveRect(GL_RECT *rect, LONG xOff, LONG yOff, LONG screen_width, LONG screen_height)
{
	LONG x_T = (*rect).x_L + xOff;
	LONG x_R = (*rect).x_R + xOff;
	LONG y_T = (*rect).y_T + yOff;
	LONG y_B = (*rect).y_B + yOff;

	if (BETWEEN(x_T, 0, screen_width) && BETWEEN(x_R, 0, screen_width) && BETWEEN(y_T, 0, screen_height) && BETWEEN(y_B, 0, screen_height))
		setRect(rect, x_T, y_T, x_R, y_B);
}

// Draw a rectangle inside a window using 2 triangles
void drawRect(LONG cx_L, LONG cy_T, LONG cx_R, LONG cy_B, LONG screen_width, LONG screen_height, LONG colour)
{
    glClear(GL_COLOR_BUFFER_BIT);	// Clear the window
    glBegin(GL_TRIANGLES);			// Draw triangles

	// Set the colour
    glColor3ub((colour & 0xFF0000) >> 16, (colour & 0x00FF00) >> 8, (colour & 0x0000FF));

	// Translate client coordinates to world coordinates
	double wx_L = CLX_TO_WRX(cx_L, screen_width);
	double wx_R = CLX_TO_WRX(cx_R, screen_width);
	double wy_T = CLY_TO_WRY(cy_T, screen_height);
	double wy_B = CLY_TO_WRY(cy_B, screen_height);

	// 1st triangle
    glVertex2d(wx_L, wy_T); // Top left
    glVertex2d(wx_R, wy_T); // Top right
	glVertex2d(wx_L, wy_B); // Bottom left

	// 2nd triangle
	glVertex2d(wx_L, wy_B); // Bottom left
    glVertex2d(wx_R, wy_B); // Bottom right
	glVertex2d(wx_R, wy_T); // Top right

	// Show the results
    glEnd();
    glFlush();
}

#endif