#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <allegro.h>

//particle structure
struct particle
{
	double mass;
	double x, y;
	long oldX, oldY;
	long xp;
	long yp;
	double ax;
	double ay;
	double vx;
	double vy;
};

int CX, CY;

void resetparticle(struct particle *p, int n);
void updateparticles();
void resetall(struct particle *p, int NUM);

void attract(struct particle *A, struct particle *B)
{
	double distance;
	double dist, distX, distY;
	double transX, transY;

	//increase position by velocity value
	A->x += A->vx;
	A->y += A->vy;

	//calculate distance between particles
	distX = A->x - B->x;
	distY = A->y - B->y;
	dist = distX * distX + distY * distY;
	if(dist != 0)
		distance = 1 / dist;
	else
		distance = 0;

	transX = distX * distance;
	transY = distY * distance;

	//acceleration = mass * distance
	A->ax = -1 * B->mass * transX;
	A->ay = -1 * B->mass * transY;

	//increase velocity by acceleration value
	A->vx += A->ax;
	A->vy += A->ay;

	//scale position to the screen
	A->xp = CX + A->x; // - p[0].x;
	A->yp = CY - A->y; // + p[0].y;
}

void update(struct particle *p, int NUM)
{
	int n;
	int i;

	//erase old particle
	for(n=0; n < NUM; n++)

	//calculate gravity for each particle
	for(n=0; n < NUM; n++)
	{
		circlefill(screen, p[n].oldX, p[n].oldY, 5, 0);

		//apply gravity between every particle
		for(i = 0; i < NUM; i++)
		{
			if(i != n)
				attract(&p[n], &p[i]);
		}

		//reset particle if it gets too far away
		if(p[n].xp < -1000 ||
			p[n].xp > 1000 ||
			p[n].yp < -1000 ||
			p[n].yp > 1000)
		{
			resetparticle(p, n);
		}

		//plot the new particle
		circlefill(screen, p[n].xp, p[n].yp, 4, 7);

		//keep track of the current position
		p[n].oldX = p[n].xp;
		p[n].oldY = p[n].yp;
	}

	//draw the primary particle
	circlefill(screen, p[0].xp, p[0].yp, 5, 15);
}

void resetparticle(struct particle *p, int n)
{
	p[n].mass = 0.001;
	p[n].ax = 0;
	p[n].ay = 0;
	p[n].xp = 0;
	p[n].yp = 0;
	p[n].x = rand() % SCREEN_W/4;
	p[n].y = rand() % SCREEN_H/4;
	p[n].vx = 0;
	p[n].vy = 0;
}

void resetall(struct particle *p, int NUM)
{
	int n;
	CX = SCREEN_W / 2;
	CY = SCREEN_H / 2;

	for(n = 0; n < NUM; n++)
		resetparticle(p, n);
}

int main(int argc, char *argv[])
{
	int NUM = 200;

	if(argc == 2 && strcmp(argv[1], "--help") == 0)
	{
		std::cout << argv[0] << "[number of particles]" << std::endl;

		return 0;
	}
	
	if(argc == 2)
	{
		NUM = atoi(argv[1]);
	}

	struct particle *p = (struct particle *)malloc(NUM * sizeof(struct particle));

	int ret;

	//initialize some stuff
	allegro_init();
	install_keyboard();
	install_timer();
	srand(time(NULL));

	//initialize video mode to 1366x768
	ret = set_gfx_mode(GFX_AUTODETECT_FULLSCREEN, 1366, 768, 0, 0);

	if(ret != 0)
	{
		allegro_message(allegro_error);
		return 1;
	}

	resetall(p, NUM);

	//wait for keypress
	while(!key[KEY_ESC])
	{
		update(p, NUM);

		textprintf_ex(screen, font, 0, 0, 15, -1,
			"Particles Program - %dx%d - Press ESC to quit",
			SCREEN_W, SCREEN_H);
	}

	//end program
	allegro_exit();
	return 0;
}
END_OF_MAIN()

