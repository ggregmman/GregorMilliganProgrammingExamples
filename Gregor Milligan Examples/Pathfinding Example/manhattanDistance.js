
//------------------Global Variables 
var cols = 50 //how many colums 
var rows = 50 //how many rows 
var grid = new Array(cols); //the grid which is an array of colums 
var openSet = []; //this is what is currently being checked
var closedSet = []; //what has already been checked 
var start;//where the algorhtm starts 
var end;//where the algorhtm starts 
var w, h;//the width and height of the window
var path = [];//the path 
var diag = true;//if diagonals are allowed or not 
//var fastestTime;
this.wall = false; //determins if the grid populates with walls 


//heuristic function 
function heuristic(a, b) {
  //euclidean
  var d = dist(a.i, a.j, b.i, b.j);
  //Manhattan Distance 
  var z = abs(a.i-b.i) + abs(a.j - b.j);
  return z; //change the return to z to use the other heuristic
}

//the setup function 
function setup() {
  
  //displays the textboxes 
  textOnScreen();
  //shows the canvas which contains the grid 
  createCanvas(800, 700);
  //shows the timer
  timer = createP('timer');
 //formating the grid related to the size of the browser window 
  w = width / cols;
  h = height / rows;

  //creating  a new array of rows that make up the grid 
  for (var i = 0; i < cols; i++) {
    grid[i] = new Array(rows);

  }
 //creating the points so the grid knows where the nodes are 
  for (var i = 0; i < cols; i++) {
    for (var j = 0; j < rows; j++) {
      grid[i][j] = new Point(i, j);

    }
  }
  //adding neighbors 
  for (var i = 0; i < cols; i++) {
    for (var j = 0; j < rows; j++) {
      grid[i][j].addNeighbors(grid);

    }
  }
  //defining the start and end goal
  
  start = grid[0][0];
  end = grid[cols - 1][rows - 1];
  
  //making sure the obstacles cant be at the start or end points on the grid 
  start.wall = false;
  end.wall = false;
  //pushes the start 
  openSet.push(start);
  //shows the grid in the console (f12 to view console) 
  console.log(grid);
  //shows the text on screen 
  function textOnScreen() {
    createP('Refresh The Page To Reset');
    createP('Press W to Remove Obstacles');
    createP('Press Q To Increase Number of Obstacles');
  }
}



//Draw function 
function draw() {
  //the timer is assigned by counting the frames of the page and then it is assigned to the textbox (paragraph) that is equal to 'timer' 
  timer.html(frameCount + ' Seconds To Solve');
  //checking where the algortim is 
  if (openSet.length > 0) {
    var winner = 0;
    for (var i = 0; i < openSet.length; i++) {
      if (openSet[i].f < openSet[winner].f) {
        winner = i;
      }
    }
    //current is the current point being checked 
    var current = openSet[winner];

    //if the current is the end (which is the goal) then stop the loop and console.log done
    if (current === end) {

      noLoop();
      console.log("done");
      //createP('Press w to remove obstacles');
      window.alert('Solved!');
    }
    //calls the remove function 
    removeFromArray(openSet, current);
    //puting the points that have already been checked into the closed set 
    closedSet.push(current);
    var neighbors = current.neighbors;
    for (var i = 0; i < neighbors.length; i++) {
      var neighbor = neighbors[i];

      //checks for neighbors and for obstacles  
      if (!closedSet.includes(neighbor) && !neighbor.wall) {
        var tempG = current.g + 1;
        var newPath = false;
        //checking which neighbors is the closest, if the old path is faster then that is the new path 
        if (openSet.includes(neighbor)) {
          if (tempG < neighbor.g) {
            neighbor.g = tempG;
            newPath = true;
          }
        } else {
          neighbor.g = tempG;
          newPath = true;
          openSet.push(neighbor);
        }
        //determining the new path if it faster than the old path and allows for the path to go through closed sets  
        if (newPath) {
          //checks how close that neighbor is to the end
          neighbor.h = heuristic(neighbor, end);
          neighbor.f = neighbor.g + neighbor.h;
          neighbor.previous = current;
        }
      }
    }

    //if the end is not found there is no solution 
  } else {
    console.log("no solution ");
    window.alert("This can't be solved");
    noLoop();
    return;
  }
  //the function that removes previously checked spots from the list 
function removeFromArray(arr, elt) {
  for (var i = arr.length - 1; i >= 0; i--) {
    if (arr[i] == elt) {
      arr.splice(i, 1);
    }
  }
}
  //the colour of the background 
  background(0);
  //this is the colour of the grid 
  for (var i = 0; i < cols; i++) {
    for (var j = 0; j < rows; j++) {
      grid[i][j].show(color(221, 17, 17));
    }
  }
  //the color of the closed set 
  for (var i = 0; i < closedSet.length; i++) {
    closedSet[i].show(color(255, 0, 0));
  }
  
  //the color of the open set 
  for (var i = 0; i < openSet.length; i++) {
    openSet[i].show(color(0, 255, 0));
  }

  //used to determine what the path is and if there is a new faster path it replaces the old one 
  path = [];
  var temp = current;
  path.push(temp);
  while (temp.previous) {
    path.push(temp.previous);
    temp = temp.previous;
  }

  //the colour of the path 
  for (var i = 0; i < path.length; i++) {
    path[i].show(color(0, 0, 255));
  }
  end.show(color(180, 17, 221));
}