#ifndef PLANET_H_
#define PLANET_H_

class Planet {
public:
  Planet(int curr_lifes = 4, int max_lifes = 4);
  ~Planet();
  void inc_lifes();
  void dec_lifes();

  //Getters
  int get_max_lifes() const;
  int get_curr_lifes() const;
private:
  int max_lifes_;
  int curr_lifes_;
};

#endif /* PLANET_H_ */
