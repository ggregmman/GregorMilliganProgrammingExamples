/*This class contains the things about each point 
such as where a point is on a grid and the neighbors 
to the grid, it also containts the wall function */ 
class Point {
    constructor(i, j) {
      this.i = i;
      this.j = j;
      this.f = 0;
      this.g = 0;
      this.h = 0;
      this.neighbors = [];
      this.previous = undefined;
      this.wall = false;
      
  
  
  
      //randomly creating a wall. 1 is the max number which would fill the whole grid with walls 
      if (random(1) < 0.3) {
        this.wall = true;
      }
  
  
  
      //This shows the grid
      this.show = function (col) {
        fill(col);
        if (this.wall) {
          fill(0); // if there is a wall it is colourd in black 
          if (keyIsDown(87)) { //87 is the keycode for w press w to delete the walls 
            this.wall = false;
            
          }
          
        }
        else if (keyIsDown(81)) { //81 is the javascript keycode for q, press q to respawn walls 
          if (random(1) < 0.1) { //adds 0.1 walls to the grid ever time that q is pressed  
            this.wall = true;
          }
        }
        
        //the grids cords
        rect(this.i * w, this.j * h, w - 1, h - 1);
      };
  
      //This is the neighbor function that allows the alg to check what points are next to eachother 
      this.addNeighbors = function (grid) {
        var i = this.i;
        var j = this.j;
        //horizontal and vertical neighbors 
        if (i < cols - 1) {
          this.neighbors.push(grid[i + 1][j]);
        }
        if (i > 0) {
          this.neighbors.push(grid[i - 1][j]);
        }
        if (j < rows - 1) {
          this.neighbors.push(grid[i][j + 1]);
        }
        if (j > 0) {
          this.neighbors.push(grid[i][j - 1]);
        }
        if (diag === true) {
          
          //diags 
          //to the left and up 
          if (i > 0 && j > 0) {
            this.neighbors.push(grid[i - 1][j - 1]);
          }
          //to the right and up
          if (i < cols - 1 && j > 0) {
            this.neighbors.push(grid[i + 1][j - 1]);
          }
          //to the left and down
          if (i > 0 && j < rows - 1) {
            this.neighbors.push(grid[i - 1][j + 1]);
          }
          //right and down 
          if (i < cols - 1 && j < rows - 1) {
            this.neighbors.push(grid[i + 1][j + 1]);
          }
        }
      };
    }
  }