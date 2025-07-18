# 1 "/home/bhaskar/study/maze_solver/maze_solver.ino"
# 2 "/home/bhaskar/study/maze_solver/maze_solver.ino" 2
# 3 "/home/bhaskar/study/maze_solver/maze_solver.ino" 2
# 4 "/home/bhaskar/study/maze_solver/maze_solver.ino" 2
# 5 "/home/bhaskar/study/maze_solver/maze_solver.ino" 2
# 6 "/home/bhaskar/study/maze_solver/maze_solver.ino" 2


// PID parameters can be adjusted as needed
Motors motor_obj;
PID pid_obj;
Sensors sens_obj;

void setup() {
  Serial.begin(9600);
  delay(1000);

  Serial.println("Self-Centering Left-Hand Maze Solver (NewPing)");
}

void loop() {

  //Read and then update the sensors
  sens_obj.update_vals();

  //Apply PID
  pid_obj.motor_change(sens_obj.left, sens_obj.right, &motor_obj);

  delay(100); // stabilize loop
}
