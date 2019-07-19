//heuristic function 
function heuristic(a, b) {
  //euclidean
  var d = dist(a.i, a.j, b.i, b.j);
  //Manhattan Distance 
  var z = abs(a.i-b.i) + abs(a.j - b.j);
  var n;
  return n; //change the return to z to use the other heuristic
  
}