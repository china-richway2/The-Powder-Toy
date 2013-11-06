#include "simulation/Elements.h"
//#TPT-Directive ElementClass Element_POWR PT_POWR 178
Element_POWR::Element_POWR()
{
	Identifier = "DEFAULT_PT_POWR";
	Name = "POWR";
	Colour = PIXPACK(0xFFFFFF);
	MenuVisible = 0;
	MenuSection = SC_POWERED;
	Enabled = 1;
	
	Advection = 0.0f;
	AirDrag = 0.00f * CFDS;
	AirLoss = 0.90f;
	Loss = 0.00f;
	Collision = 0.0f;
	Gravity = 0.0f;
	Diffusion = 0.00f;
	HotAir = 0.000f	* CFDS;
	Falldown = 0;
	
	Flammable = 0;
	Explosive = 0;
	Meltable = 0;
	Hardness = 1;
	
	Weight = 100;
	
	Temperature = R_TEMP+0.0f	+273.15f;
	HeatConduct = 251;
	Description = "Powered material. Draw PRTI or PRTO on it to let it act like the element.";
	
	State = ST_NONE;
	Properties = TYPE_SOLID;
	
	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = ITH;
	HighTemperatureTransition = NT;
	
	Update = &Element_POWR::update;
	Graphics = &Element_POWR::graphics;
}

//#TPT-Directive ElementHeader Element_POWR static int update(UPDATE_FUNC_ARGS)
int Element_POWR::update(UPDATE_FUNC_ARGS)
{
	 if (parts[i].ctype != 0)
		 return (*(sim->elements[parts[i].ctype].Update))(UPDATE_FUNC_SUBCALL_ARGS);
	 return 1;
}


//#TPT-Directive ElementHeader Element_POWR static int graphics(GRAPHICS_FUNC_ARGS)
int Element_POWR::graphics(GRAPHICS_FUNC_ARGS)
{
	if (cpart->ctype != 0)
	{
		//Create a temp. particle and do a subcall.
		Particle tpart;
		int t;
		memset(&tpart, 0, sizeof(Particle));
		tpart.type = cpart->ctype;
		tpart.temp = cpart->temp;
		tpart.life = cpart->life;
		tpart.tmp = cpart->tmp;
		tpart.ctype = 0;
		if (tpart.type == PT_PHOT && tpart.ctype == 0x40000000)
			tpart.ctype = 0x3FFFFFFF;
		t = tpart.type;
		if (ren->graphicscache[t].isready)
		{
			*pixel_mode = ren->graphicscache[t].pixel_mode;
			*cola = ren->graphicscache[t].cola;
			*colr = ren->graphicscache[t].colr;
			*colg = ren->graphicscache[t].colg;
			*colb = ren->graphicscache[t].colb;
			*firea = ren->graphicscache[t].firea;
			*firer = ren->graphicscache[t].firer;
			*fireg = ren->graphicscache[t].fireg;
			*fireb = ren->graphicscache[t].fireb;
		}
		else
		{
			*colr = PIXR(ren->sim->elements[t].Colour);
			*colg = PIXG(ren->sim->elements[t].Colour);
			*colb = PIXB(ren->sim->elements[t].Colour);
			if (ren->sim->elements[t].Graphics)
			{
				return (*(ren->sim->elements[t].Graphics))(ren, &tpart, nx, ny, pixel_mode, cola, colr, colg, colb, firea, firer, fireg, fireb);
			}
			else
			{
				return Element::defaultGraphics(ren, &tpart, nx, ny, pixel_mode, cola, colr, colg, colb, firea, firer, fireg, fireb);
			}
		}
	}
	else
		return 0;
}


Element_POWR::~Element_POWR() {}
