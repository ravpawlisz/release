#include<allegro.h>
#include<string>
#include<cmath>
#include<time.h>
#define boardx 1280+8
#define boardy 720+8
using std::string;
void introduction(), preparation(int board[][boardy], float&, float&, int&, float&), counting(float&, float&, int, float&), fruit(int board [][boardy], float, float, int&), movement(int board[][boardy], float&, float&, int), displaying(BITMAP*, int board[][boardy]), termination(BITMAP*);
bool menu(), obstacle(int board[][boardy], float, float);
int color(string), round(float);

int main(){
	int board[boardx][boardy], length; //actual lenght is 9 fewer
	float headx, heady, angle; //predefined location of the snake's head
	srand(time(NULL)); //setting a seed on time dependence
	introduction(); //preparing the program
	BITMAP* buffer=create_bitmap(boardx-8,boardy-8); //creating a buffer
	while(menu()){ //displaying a menu till exit the game
		preparation(board, headx, heady, length, angle);
		while(obstacle(board, headx, heady)){ //a game loop; checking if snake bumped
			counting(headx, heady, length, angle);
			fruit(board, headx, heady, length); //checking if snake ate fruit
			movement(board, headx, heady,length);
			displaying(buffer, board);
		}
	}
	termination(buffer); //finishing the program
	return 0;
}
END_OF_MAIN();

void introduction(){ //preparing the program
	allegro_init();
	set_color_depth(32);
	set_gfx_mode(GFX_AUTODETECT_WINDOWED,boardx-8,boardy-8,0,0);
	install_keyboard();
}

void preparation(int board[boardx][boardy], float &headx, float &heady, int &length, float &angle){ //preparing the game
	for(int y=0;y<boardy;++y) //cleaning the board; the whole board is setting as a background
		for(int x=0;x<boardx;++x)
			board[x][y]=0;

	headx=(boardx-8)/2;
	heady=(boardy-8)/2;
	length=10;
	angle=0;
	board[int(headx)][int(heady)]=10; //fixing the snake's head

	//temp
	board[rand()%300+300][rand()%300+300]=2;
	board[500][300]=1;
}

bool menu(){ //handling a menu
	//allegro_message("wanna play? zero means no");
	//int x=readkey();
	//if(x==6960) return false;
	//else return true;

	allegro_message("wanna play? 1 means yes");
	rest(2000);
	if(key[KEY_1]) return true;
	return false;
}

void counting(float &headx, float &heady, int length, float &angle){
	//rotation / speeding; counting new head position
	float speed=1;
	if(key[KEY_LEFT]) angle-=1./128;
	if(key[KEY_RIGHT]) angle+=1./128;
	if(key[KEY_UP]) speed=2;
	if(key[KEY_DOWN]) speed=0.5;
	headx+=cos(angle*3.1415926535*2)*speed;
	heady+=sin(angle*3.1415926535*2)*speed;

	//teleportation between edges of the board
	if(headx<4) headx+=boardx-8;
	if(headx>boardx-4) headx-=boardx-8;
	if(heady<4) heady+=boardy-8;
	if(heady>boardy-4) heady-=boardy-8;
}

void fruit(int board[boardx][boardy], float headx, float heady, int &length){
	for(int y=heady-4;y<=heady+4;++y){
		for(int x=headx-4;x<headx+4;++x){
			if(board[x][y]==1){
				board[x][y]=0;
				length+=5;
				board[rand()%(boardx-4)+4][rand()%(boardy-4)+4]=1;
			}
		}
	}
}

void movement(int board[boardx][boardy], float &headx, float &heady, int length){
	//fixing the new postion on the board
	board[round(headx)][round(heady)]=length+1;
	for(int y=4;y<boardy-4;++y)
		for(int x=4;x<boardx-4;++x)
			if(board[x][y]>=10)
				board[x][y]-=1;

	rest(0); //delay between steps
}

int round(float x){
	return floor(x+0.5);
}

void displaying(BITMAP* buffer, int board[boardx][boardy]){
	clear_to_color(buffer,color("bg")); //drawing the background
	for(int y=4;y<boardy-4;++y){
		for(int x=4;x<boardx-4;++x){
			if(board[x][y]>=10) //drawing the snake
				circlefill(buffer,x-4,y-4,4,color("snake"));
			if(board[x][y]==1) //drawing the fruit
				circlefill(buffer,x-4,y-4,4,color("fruit"));
			if(board[x][y]==2) //drawing the obstacles
				circlefill(buffer,x-4,y-4,4,color("obstacle"));
		}
	}
	blit(buffer,screen,0,0,0,0,boardx-8,boardy-8); //copying the buffer to the screen; actual displaying
}

int color(string x){ //making default colors
	if(x=="bg") return makecol(150,200,0); //backround color
	if(x=="snake") return makecol(50,70,10);
	if(x=="fruit") return makecol(255,0,0);
	if(x=="obstacle") return makecol(0,0,0);
	return 0;
}

bool obstacle(int board[boardx][boardy], float headx, float heady){
	for(int y=heady-4;y<=heady+4;++y){
		for(int x=headx-4;x<headx+4;++x){
			if(board[x][y]==2){
				board[x][y]=0;
				return false;
			}
		}
	}
	return true;
}

void termination(BITMAP* buffer){ //finishes the program
	destroy_bitmap(buffer);
	allegro_exit();
}