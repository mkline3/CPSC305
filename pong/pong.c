/*
 * pong.c
 * this program modifies square.c to have a paddle on both sides of the screen
 * there will be "ball" that will bounce between paddles till one scores
 */

/* the width and height of the screen */
#define WIDTH 240
#define HEIGHT 160

/* these identifiers define different bit positions of the display control */
#define MODE4 0x0004
#define BG2 0x0400

/* this bit indicates whether to display the front or the back buffer
 * this allows us to refer to bit 4 of the display_control register */
#define SHOW_BACK 0x10;

/* the screen is simply a pointer into memory at a specific address this
 *  * pointer points to 16-bit colors of which there are 240x160 */
volatile unsigned short* screen = (volatile unsigned short*) 0x6000000;

/* the display control pointer points to the gba graphics register */
volatile unsigned long* display_control = (volatile unsigned long*) 0x4000000;

/* the address of the color palette used in graphics mode 4 */
volatile unsigned short* palette = (volatile unsigned short*) 0x5000000;

/* pointers to the front and back buffers - the front buffer is the start
 * of the screen array and the back buffer is a pointer to the second half */
volatile unsigned short* front_buffer = (volatile unsigned short*) 0x6000000;
volatile unsigned short* back_buffer = (volatile unsigned short*)  0x600A000;

/* the button register holds the bits which indicate whether each button has
 * been pressed - this has got to be volatile as well
 */
volatile unsigned short* buttons = (volatile unsigned short*) 0x04000130;

/* the bit positions indicate each button - the first bit is for A, second for
 * B, and so on, each constant below can be ANDED into the register to get the
 * status of any one button */
#define BUTTON_A (1 << 0)
#define BUTTON_B (1 << 1)
#define BUTTON_SELECT (1 << 2)
#define BUTTON_START (1 << 3)
#define BUTTON_RIGHT (1 << 4)
#define BUTTON_LEFT (1 << 5)
#define BUTTON_UP (1 << 6)
#define BUTTON_DOWN (1 << 7)
#define BUTTON_R (1 << 8)
#define BUTTON_L (1 << 9)

/* the scanline counter is a memory cell which is updated to indicate how
 * much of the screen has been drawn */
volatile unsigned short* scanline_counter = (volatile unsigned short*) 0x4000006;

/* wait for the screen to be fully drawn so we can do something during vblank */
void wait_vblank() {
    /* wait until all 160 lines have been updated */
    while (*scanline_counter < 160) { }
}

/* this function checks whether a particular button has been pressed */
unsigned char button_pressed(unsigned short button) {
    /* and the button register with the button constant we want */
    unsigned short pressed = *buttons & button;

    /* if this value is zero, then it's not pressed */
    if (pressed == 0) {
        return 1;
    } else {
        return 0;
    }
}

/* keep track of the next palette index */
int next_palette_index = 0;

/*
 * function which adds a color to the palette and returns the
 * index to it
 */
unsigned char add_color(unsigned char r, unsigned char g, unsigned char b) {
    unsigned short color = b << 10;
    color += g << 5;
    color += r;

    /* add the color to the palette */
    palette[next_palette_index] = color;

    /* increment the index */
    next_palette_index++;

    /* return index of color just added */
    return next_palette_index - 1;
}

/* a colored square */
struct square {
    unsigned short x, y, size;
    unsigned char color;
};

/* put a pixel on the screen in mode 4 */
void put_pixel(volatile unsigned short* buffer, int row, int col, unsigned char color) {
    /* find the offset which is the regular offset divided by two */
    unsigned short offset = (row * WIDTH + col) >> 1;

    /* read the existing pixel which is there */
    unsigned short pixel = buffer[offset];

    /* if it's an odd column */
    if (col & 1) {
        /* put it in the left half of the short */
        buffer[offset] = (color << 8) | (pixel & 0x00ff);
    } else {
        /* it's even, put it in the left half */
        buffer[offset] = (pixel & 0xff00) | color;
    }
}

/* draw a square onto the screen */
void draw_square(volatile unsigned short* buffer, struct square* s) {
    short row, col;
    /* for each row of the square */
    for (row = s->y; row < (s->y + s->size); row++) {
        /* loop through each column of the square */
        for (col = s->x; col < (s->x + 2); col++) {
            /* set the screen location to this color */
            put_pixel(buffer, row, col, s->color);
        }
    }
}

/* Draw ball to screen */
void draw_ball(volatile unsigned short* buffer, struct square* s) {
    short row, col;
    /* for each row of the square */
    for (row = s->y; row < (s->y + s->size); row++) {
        /* loop through each column of the square */
        for (col = s->x; col < (s->x + s->size); col++) {
            /* set the screen location to this color */
            put_pixel(buffer, row, col, s->color);
        }
    }
}

/* clear the screen right around the square */
void update_screen(volatile unsigned short* buffer, unsigned short color, struct square* s, struct square* c, struct square* b) {
    short row, col;
    for (row = s->y - 3; row < (s->y + s->size + 3); row++) {
        for (col = s->x - 3; col < (s->x + s->size + 3); col++) {
            put_pixel(buffer, row, col, color);
        }
    }
    for (row = c->y - 3; row < (c->y + c->size + 3); row++) {
        for (col = c->x - 3; col < (c->x + s->size + 3); col++) {
            put_pixel(buffer, row, col, color);
        }
    }
    for (row = b->y - 3; row < (b->y + c->size + 3); row++) {
        for (col = b->x - 3; col < (b->x + s->size + 3); col++) {
            put_pixel(buffer, row, col, color);
        }
    }
}

/* this function takes a video buffer and returns to you the other one */
volatile unsigned short* flip_buffers(volatile unsigned short* buffer) {
    /* if the back buffer is up, return that */
    if(buffer == front_buffer) {
        /* clear back buffer bit and return back buffer pointer */
        *display_control &= ~SHOW_BACK;
        return back_buffer;
    } else {
        /* set back buffer bit and return front buffer */
        *display_control |= SHOW_BACK;
        return front_buffer;
    }
}

/* handle the buttons which are pressed down */
void handle_buttons(struct square* s) {
    /* move the square with the arrow keys */
    if (button_pressed(BUTTON_DOWN)) {
	if(s->y != HEIGHT -18)
        	s->y += 1;
    }
    if (button_pressed(BUTTON_UP)) {
	if(s->y != 0)
        	s->y -= 1;
    }
   /* if (button_pressed(BUTTON_RIGHT)) {
        s->x += 1;
    }
    if (button_pressed(BUTTON_LEFT)) {
        s->x -= 1;
    }*/
}

/* modifiers for movement */
int move = 1;
void set_m(int m){
	move = m;
}
int get_move(){
	return move;
}
/* handles movement of cpu paddle */
void basic_move(struct square* c){
	if(get_move() == 1){
		c->y += 1;
		if(c->y == HEIGHT -18)
			set_m(0);
	}
	if(get_move() == 0){
		c->y -=1;
		if(c->y == 0)
			set_m(1);
	}
}
int by = 0;
int balling = 0;
void advanced_move(struct square* c, struct square* b){
	if(balling == 1){
		if(b->y > (c->y +( c->size /2)+6)){
			if(c->y != HEIGHT-33)//creates sweet spot in corrner to score
				c->y +=1;
		}
		
		
		if(b->y < (c->y + (c->size /2)-6)){
			if(c->y != 15)// creates other sweet spot
				c->y -=1;
		}
	
	}
}
/*handles movement for the ball*/
//int balling = 0;
int score = 0;
int bx = 0;
int contact = 0;
void ball_move(struct square* b){
	if(b->y == 3)
		by = 1;
	if(b->y == HEIGHT-2)
		by = -1;
	if(balling == 0){
		b->x -=1;
		b->y +=by;
		if(contact == 1)
			balling = 1;
	}
	 if(balling == 1){
		b->x +=1;
		b->y +=by;
		if( contact == 2) 
			balling = 0;
	} 
/*	if(b->x == 0 || b->x == WIDTH - 3){
		b->x = WIDTH/2;
		b->y = HEIGHT/2;
		by = 0;
		balling = 0;
	}*/	

}
int scored = 0;
/*checks to see if the ball hits the sides*/
void score_check(struct square* b,volatile unsigned short* front_buffer,volatile unsigned short* back_buffer, unsigned short color){
	short row, col;
	if(b->x == 0 || b->x == WIDTH - 3){
		 for (row = 0; row <HEIGHT; row++) {
     		   for (col = b->x; col < (b->x + b->size + 3); col++) {
            		put_pixel(front_buffer, row, col, color);
			put_pixel(back_buffer, row, col, color);
       		   }
   		 }
		 for (row = 0; row <HEIGHT; row++) {
                   for (col = WIDTH-12; col < WIDTH; col++) {
                        put_pixel(front_buffer, row, col, color);
                        put_pixel(back_buffer, row, col, color);
                   }
                 }
                b->x = WIDTH/2;
                b->y = HEIGHT/2;
                by = 0;
		//clear_screen(front_buffer, color);
               // clear_screen(back_buffer, color);
               
               // balling = 0;
		scored = 1;
        }
}
/*handles contact between ball and paddles */
void paddle_hit(struct square* s, struct square* c, struct square* b){
	int j=0;
	int i=0;
	contact = 0;
	while( i !=1){
		
		if((s->y + j) == b->y && (s->x +1) == b->x){
			if(5< j < 11){
				by = 0;
				//bx = 0;
			}
			if(j<6){
				by = -1;
				//bx = -1
			}
			if(j>10){
				by = 1;
				//bx = 1;
			}
			contact= 1;
			i = 1;
		}
		if((c->y +j) == (b->y -3) && (c->x - 2) == b->x){
			if(5<j<11){
                                by = 0;
                                //bx = 0;
                        }
                        if(j<6){
                                by = -1;
                               // bx = -1
                        }
                        if(j>10){
                                by = 1;
               
                        }
			contact = 2;
			i = 1;
		}
		j++;
		if(j  == s->size)
			i = 1;
	}
}
/* clear the screen to black */
void clear_screen(volatile unsigned short* buffer, unsigned short color) {
    unsigned short row, col;
    /* set each pixel black */
    for (row = 0; row < HEIGHT; row++) {
        for (col = 0; col < WIDTH; col++) {
            put_pixel(buffer, row, col, color);
        }
    }
}

/* the main function */
int main() {
    /* we set the mode to mode 4 with bg2 on */
    *display_control = MODE4 | BG2;

    /* make player paddle green */
    struct square s = {10, 10, 18, add_color(2, 2, 25)};
    /* make cpu paddle */
    struct square c = {WIDTH - 20, 10, 18, add_color(20,2,2)};
    /* make ball for pong */
    struct square b = {(WIDTH/2),(HEIGHT/2), 3, add_color(31,31,31)};
    /* add black to the palette */
    unsigned char black = add_color(0, 0, 0);

    /* the buffer we start with */
    volatile unsigned short* buffer = front_buffer;

    /* clear whole screen first */
    clear_screen(front_buffer, black);
    clear_screen(back_buffer, black);
    /* loop forever */
    while (1) {
        /* clear the screen - only the areas around the square! */
        update_screen(buffer, black, &s,&c,&b);

        /* draw the square */
        draw_square(buffer, &s);
	draw_square(buffer, &c);
	draw_ball(buffer, &b);

        /* handle button input */
        handle_buttons(&s);

        /* wiat for vblank before switching buffers */
        wait_vblank();
	
	/*movement for cpu paddle */
	//basic_move(&c);
	advanced_move(&c,&b);
    	/*ball movement*/
	paddle_hit(&s,&c,&b);
	ball_move(&b);
	score_check(&b, front_buffer, back_buffer, black);
        /* swap the buffers */
        buffer = flip_buffers(buffer);
    }
}

/* the game boy advance uses "interrupts" to handle certain situations
 * for now we will ignore these */
void interrupt_ignore() {
    /* do nothing */
}

/* this table specifies which interrupts we handle which way
 * for now, we ignore all of them */
typedef void (*intrp)();
const intrp IntrTable[13] = {
    interrupt_ignore,   /* V Blank interrupt */
    interrupt_ignore,   /* H Blank interrupt */
    interrupt_ignore,   /* V Counter interrupt */
    interrupt_ignore,   /* Timer 0 interrupt */
    interrupt_ignore,   /* Timer 1 interrupt */
    interrupt_ignore,   /* Timer 2 interrupt */
    interrupt_ignore,   /* Timer 3 interrupt */
    interrupt_ignore,   /* Serial communication interrupt */
    interrupt_ignore,   /* DMA 0 interrupt */
    interrupt_ignore,   /* DMA 1 interrupt */
    interrupt_ignore,   /* DMA 2 interrupt */
    interrupt_ignore,   /* DMA 3 interrupt */
    interrupt_ignore,   /* Key interrupt */
};

