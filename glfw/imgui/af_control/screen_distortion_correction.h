namespace screen_distortion_correction{
  void init(float width,float height);
  void uninit();
  void bind();
  void disbind();
  void draw();
  void calculate_params(float input_point[15][2],float output_point[15][2]);
}