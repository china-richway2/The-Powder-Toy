#include "simulation/Elements.h"
//#TPT-Directive ElementClass Element_LANG PT_LANG 177
Element_LANG::Element_LANG()
{
	Identifier = "DEFAULT_PT_LANG";
	Name = "LANG";
	Colour = PIXPACK(0x00FF00);
	MenuVisible = 1;
	MenuSection = SC_SPECIAL;
	Enabled = 1;
	
	Advection = 0.0f;
	AirDrag = 0.00f * CFDS;
	AirLoss = 1.00f;
	Loss = 0.00f;
	Collision = 0.0f;
	Gravity = 0.0f;
	Diffusion = 0.00f;
	HotAir = -0.0003f* CFDS;
	Falldown = 0;
	
	Flammable = 0;
	Explosive = 0;
	Meltable = 0;
	Hardness = 0;
	
	Weight = 100;
	
	Temperature = R_TEMP+0.0f	+273.15f;
	HeatConduct = 0;
	Description = "Langton's Ant. Does not conduct heat. See http://en.wikipedia.org/wiki/Langtons_ant.";
	
	State = ST_SOLID;
	Properties = TYPE_SOLID;
	
	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = ITH;
	HighTemperatureTransition = NT;
	
	Update = &Element_LANG::update;
	Graphics = &Element_LANG::graphics;
}

//#TPT-Directive ElementHeader Element_LANG static int update(UPDATE_FUNC_ARGS)
int Element_LANG::update(UPDATE_FUNC_ARGS)
{
	int tmp = sim->parts[i].tmp;
	int vx, vy;
	int black = tmp & 0x10;
	int newblack = 0;
	if ((tmp & 0x04) == 0) return 0;
	// decide whether we should turn left or right, -1 for left and +1 for right
	// turn left if black and turn right if white
	tmp = (black ? tmp - 1 + 4: tmp + 1) & 0x03;
	switch (tmp & 0x03)
	{
	case 0:
		vx = 1;
		vy = 0;
		break;
	case 1:
		vx = 0;
		vy = 1;
		break;
	case 2:
		vx = -1;
		vy = 0;
		break;
	case 3:
		vx = 0;
		vy = -1;
		break;
	}
	int newx = vx + x, newy = vy + y;
	int newi = sim->pmap[newy][newx];
	if (newi & 0xFF == PT_LANG && parts[newi>>8].tmp == 0x10) // new position has a black square
		newblack = 1;
	else if (newi == 0) // new position has nothing
		newblack = 0;
	else //new position has been blocked
		newblack = 1;
		//return 0; // do nothing
	// if the new position has a black square
	if (newblack) {
		sim->kill_part(newi >> 8);
	}
	// go to new position
	sim->parts[i].tmp = tmp | 4 | newblack << 4;
	sim->parts[i].x = newx;
	sim->parts[i].y = newy;
	sim->pmap[y][x] = 0;
	sim->pmap[newy][newx] = i >> 8 | PT_LANG;
	if (black == 0) {
		// If the last position is white (should be black after that), leave a black square
		int newAnt = sim->create_part(-1, x, y, PT_LANG);
		sim->parts[newAnt].tmp = 0x10;
	}
	return 0;
}
	
//#TPT-Directive ElementHeader Element_LANG static int graphics(GRAPHICS_FUNC_ARGS)
int Element_LANG::graphics(GRAPHICS_FUNC_ARGS)
{
	if((cpart->tmp & 0x04) == 0)
	{
		*colr = 0xFF;
		*colg = 0;
	}
	return 0;
}

	
Element_LANG::~Element_LANG() {}