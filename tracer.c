#include <stdio.h>
#include <math.h>
#include <ncurses.h>
#include <stdlib.h>

#define NUM_SPHERES 2
#define RESOLUTION 0.02
#define CAM_OFFSET 2

const int max_x, max_y;

struct Vec{
   double x,y,z;
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
    const int width, height;
    struct Vec **pixels;
    struct Sphere *spheres;
};

struct Image *img;

// Caller handles memory
struct Vec * compute_direction(Vec start, Vec end){
    struct Vec *returnVec = malloc(sizeof(struct Vec));
    returnVec->x = end.x - start.x;
    returnVec->y = end.y - start.y;
    returnVec->z = end.z - start.z;
    return returnVec;
};

void init_image(struct Image *img){
    for(int i = 0; i < img->width; i++){
        for(int j = 0; j < img->height; j++){
            pixels[i][j].x = cam->x - (((double)img->width * RESOLUTION)/ 2 ) + i * RESOLUTION;
	    pixels[i][j].y = cam->y - (((double)img->height * RESOLUTION)/ 2 ) + j * RESOLUTION;
	    pixels[i][j].z = cam->z + CAM_OFFSET;
	}
    }
}

void render_image(){
    for(int i = 0; i < img->width; i++){
       for(int j = 0; j < img->height; j++){
           struct Vec *primary_dir = compute_direction(*cam, );
       }
    }
}

void vec_multiply(struct Vec *vec, double mult){
      vec->x *= mult;
      vec->y *= mult;
      vec->z *= mult;
}

double vec_product(struct Vec vec1, struct Vec vec2){
      return vec1.x * vec2.x + vec1.y * vec2.y + vec3.z * vec3.z;
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
      struct Vec *diff_start_end = vec_difference(origin, sphere.center);
      return pow(vec_product(start, *diff_start_end),2) - pow(vec_norm(diff_start_end),2) + pow(sphere.radius,2);
}

void free_everything(){
    free(img->spheres);
    free(img->pixels);
    free(img);
}

int main(){
	 initscr();
	 noecho();
	 cbreak();
	 curs_set(FALSE);
         getmaxyx(stdscr, max_y, max_x);
	 nodelay(stdscr, TRUE);
         img = malloc(sizeof(struct Image));
	 img->pixels = malloc(max_x * max_y * sizeof(struct Vec));
	 img->width = max_x;
	 img->height = max_y;
	 img->spheres = malloc(NUM_SPHERES * sizeof(struct Sphere));
}
