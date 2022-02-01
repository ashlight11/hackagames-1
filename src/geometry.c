/*******************************************************************************************
*
*   HACKAGAME
*   Copyright (c) 2021-2022 Guillaume Lozenguez - Institut Mines-Telecom
*
********************************************************************************************/

#include "geometry.h"

#include <stdio.h>
#include <math.h>

/* tools for RayLib Float2 */

unsigned Color_red(unsigned color)
{
    return (color & 0xFF000000) >> 24;
}
unsigned Color_green(unsigned color)
{
    return (color & 0x00FF0000) >> 16;
}
unsigned Color_blue(unsigned color)
{
    return (color & 0x0000FF00) >> 8;
}

unsigned Color_alpha(unsigned color)
{
    return (color & 0x000000FF);
}

void Float2_str(Float2 self, char* buffer)
{
    sprintf( buffer, "[%.2f, %.2f]", self.x, self.y );
}

float Float2_length2(Float2 self)
{
    return (self.x*self.x) + (self.y*self.y);
}

float Float2_length(Float2 self)
{
    return sqrtf( Float2_length2(self) );
}

float Float2_distance2(Float2 self, Float2 another)
{
    return (another.x - self.x) * (another.x - self.x) + (another.y - self.y) * (another.y - self.y);
}

float Float2_distance(Float2 self, Float2 another)
{
    return sqrtf( Float2_distance2(self, another) );
}

float Float2_normalize( Float2 *vect )
{
    float d= Float2_length( *vect );
    vect->x/= d;
    vect->y/= d;
    return d;
}

void Float2_repultion(Float2 *self, Float2 *another, float dist)
{
    Float2 vect= {
        (self->x - another->x),
        (self->y - another->y)
    };
    float ddist= 0.5001f * dist;
    Float2_normalize( &vect );
    self->x+= vect.x * ddist;
    self->y+= vect.y * ddist;
    another->x+= -vect.x * ddist;
    another->y+= -vect.y * ddist;
}
