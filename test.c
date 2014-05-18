#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <err.h>


#define E2ID(iev)  ((short) (iev.num << 8) | (iev.type << 0))
#define V2ID(n, t) ((short) (      n << 8) | (       t << 0))

struct input_event {
	unsigned int  ts;
	short         val;
	unsigned char type;
	unsigned char num;
};

struct f710 {
	double lx;
	double ly;
	double rx;
	double ry;
	double lt;
	double rt;
	int lb:1;
	int rb:1;
	int left:1;
	int right:1;
	int up:1;
	int down:1;
	int a:1;
	int b:1;
	int x:1;
	int y:1;
	int start:1;
	int back:1;
	int logitech:1;
	int ljb:1;
	int rjb:1;
};

void print_f710(struct f710 *c)
{
	printf(" __________                         _________ \n"
               "|    %3.1f   |                       |   %3.1f   |\n"
               "|    %u     |                       |   %u     |\n"
               "|--------------------------------------------|\n"
               "|    %u           %u    %u    %u           %u     |\n"
               "| %u     %u        x         x        %u     %u  |\n"
               "|    %u                                 %u     |\n"
               "|          %+3.1f               %+3.1f           |\n"
               "|          %+3.1f               %+3.1f           |\n"
               "|           %u                  %u             |\n",
               c->lt, c->rt, !!c->lb, !!c->rb, !!c->up, !!c->back,
               !!c->logitech,!!c->start, !!c->y, !!c->left,
               !!c->right, !!c->x, !!c->b, !!c->down, !!c->a, c->lx,
               c->rx, c->ly, c->ry, !!c->ljb, !!c->rjb);
}

int main(int argc, char **argv)
{
	struct input_event iev;
	int js;
	struct f710 c = {0};

	js = open("/dev/input/js0", O_RDONLY);
	if (js < 0)
		err(1, "open()");

	for (;;) {
		int ret;

		ret = read(js, &iev, sizeof(iev));
		if (ret != sizeof(iev))
			err(1, "read()");
		switch (E2ID(iev)) {
		case V2ID(0, 1): c.a  = !!iev.val;                       break;
		case V2ID(0, 2): c.lx = (iev.val / 32767.0);             break;
		case V2ID(1, 1): c.b  = !!iev.val;                       break;
		case V2ID(1, 2): c.ly = (iev.val / 32767.0);             break;
		case V2ID(2, 1): c.x  = !!iev.val;                       break;
		case V2ID(2, 2): c.lt = (iev.val + 32767.0) / (2*32767); break;
		case V2ID(3, 1): c.y  = !!iev.val;			 break;
		case V2ID(3, 2): c.rx = iev.val / 32767.0;		 break;
		case V2ID(4, 1): c.lb = !!iev.val;			 break;
		case V2ID(4, 2): c.ry = iev.val / 32767.0;		 break;
		case V2ID(5, 1): c.rb = !!iev.val;			 break;
		case V2ID(5, 2): c.rt = (iev.val + 32767.0) / (2*32767); break;
		case V2ID(6, 1): c.back = !!iev.val;			 break;
		case V2ID(6, 2):
			c.left  = (iev.val == -32767);
			c.right = (iev.val ==  32767);
                        break;
		case V2ID(7, 1): c.start = !!iev.val;			 break;
		case V2ID(7, 2):
			c.up   = (iev.val == -32767);
			c.down = (iev.val ==  32767);
                        break;
		case V2ID(8, 1): c.logitech		= !!iev.val;     break;
		case V2ID(9, 1): c.ljb			= !!iev.val;     break;
		case V2ID(10, 1): c.rjb			= !!iev.val;     break;
		}
		printf("code: %u\ttype: %u\tval: %d\n",
                       iev.num, iev.type, iev.val);
		print_f710(&c);
	}
}
