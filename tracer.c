#include <stdio.h>
#include <math.h>
#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_SPHERES 1
#define RESOLUTION 0.02
#define CAM_OFFSET 2
#define MIN_DIST 5000

int max_x, max_y;

struct Vec{
   double x,y,z;
   int color;
};

struct Line{
   struct Vec start;
   struct Vec end;
};

struct Sphere{
   struct Vec center;
   double radius;
};

struct Vec *cam;

struct Image{
    int width, height;
    struct Vec **pixels;
    struct Sphere *spheres;
};

struct Image *img;

// Caller handles memory
struct Vec * compute_direction(struct Vec start,struct Vec end){
    struct Vec *returnVec = malloc(sizeof(struct Vec));
    returnVec->x = end.x - start.x;
    returnVec->y = end.y - start.y;
    returnVec->z = end.z - start.z;
    return returnVec;
};

void init_image(){
    for(int i = 0; i < img->width; i++){
        for(int j = 0; j < img->height; j++){
            img->pixels[i][j].x = cam->x - (((double)img->width * RESOLUTION)/ 2 ) + i * RESOLUTION;
	    img->pixels[i][j].y = cam->y - (((double)img->height * RESOLUTION)/ 2 ) + j * RESOLUTION;
	    img->pixels[i][j].z = cam->z + CAM_OFFSET;
	}
    }
}

struct Vec *create_vec(int x, int y, int z){
     struct Vec *vec = malloc(sizeof(struct Vec));
     vec->x = x;
     vec->y = y;
     vec->z = z;
     return vec;
}

void vec_multiply(struct Vec *vec, double mult){
      vec->x *= mult;
      vec->y *= mult;
      vec->z *= mult;
}

double vec_product(struct Vec vec1, struct Vec vec2){
      return vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z;
}

//Caller handles memory
struct Vec * vec_difference(struct Vec v2, struct Vec v1){
	struct Vec *return_vec = malloc(sizeof(struct Vec));
        return_vec->x = v2.x - v1.x;
        return_vec->y = v2.y - v1.y;
        return_vec->z = v2.z - v1.z;
	return return_vec;
}

double vec_norm(struct Vec vec){
         return sqrt(pow(vec.x,2) + pow(vec.y,2) + pow(vec.z,2));
}

double sphere_line_intersection(struct Sphere sphere, struct Vec vec, struct Vec origin){

      // Calculate unit vector of vec
      
      double length = vec_norm(vec);
      vec_multiply(&vec, 1/length);
      struct Vec *diff_p = vec_difference(origin, sphere.center);
      struct Vec diff_start_end = *diff_p;
      double result = pow(vec_product(vec, diff_start_end),2) - pow(vec_norm(diff_start_end),2) + pow(sphere.radius,2); 
      free(diff_p);
      return result;
}

void render_image(){
    clear();
    for(int i = 0; i < img->width; i++){
       for(int j = 0; j < img->height; j++){
           struct Vec *primary_dir = compute_direction(*cam, img->pixels[i][j]);
	   struct Vec p_hit;
	   struct Vec normal_hit;
	   double min_dist = MIN_DIST;

           for(int k = 0; k < NUM_SPHERES; ++k){
                 if(sphere_line_intersection(*(img->spheres + k), *primary_dir, *cam) >= 0){
                     img->pixels[i][j].color = 1;
		     mvprintw(j,i,".");
		 }
		 else{
                     img->pixels[i][j].color = 0;
		 }
	   }
	   free(primary_dir);
       }
    }
    refresh();
    usleep(500000);
}

void free_everything(){
    free(img->spheres);
    free(img->pixels);
    free(img);
    free(cam);
}

int main(){
	 initscr();
	 noecho();
	 cbreak();
	 curs_set(FALSE);
         getmaxyx(stdscr, max_y, max_x);
	 nodelay(stdscr, TRUE);
         img = malloc(sizeof(struct Image));
	 img->width = max_x;
	 img->height = max_y;
	 img->pixels = malloc(max_x * sizeof(*img->pixels));
	 if(img->pixels){
            for(int i = 0; i < max_x; i++){
               img->pixels[i] = malloc(sizeof(*img->pixels[i]) * max_y);
	    }
	 }
	 img->spheres = malloc(NUM_SPHERES * sizeof(struct Sphere));
	 struct Vec *center_vec = create_vec(0,0,-5);
	 img->spheres->center = *center_vec;
	 img->spheres->radius = 5;
	 cam = create_vec(0,0,-10); 
	 init_image();
	 for(int i = 0; i < 100; i ++){
            img->spheres->center.z++;
	    render_image();
	 }
	 free(center_vec);
	 free_everything();
}
