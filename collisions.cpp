// Matthew Fernandez
// 2014
//
// State machine simulating how many collisions are going to occur.

#include <random>
#include <vector>
#include <iostream>
#include <cstdio>

#define USER_SIZE 50
#define TIME_THRESHOLD 2000 // maximum wait till transmission in ms
#define SIMULATION_TIME 7200 // how long for sim in seconds
#define TX_TIME 25 // average time it takes to transmit a message in ms

enum State { TXING, IDLE};

struct User {
  State state;
  int time_till_tx;
  int tx_progress;
};

int main() {
  // set up PRNG
  std::random_device rd;
  std::mt19937_64 gen(rd());
  std::uniform_int_distribution<> dist(1, TIME_THRESHOLD);

  // the 1th element is how many 'ticks' there were colisions during
  std::vector<int> collisions;

  User users[USER_SIZE];

  // initialize our users
  for( int i = 0; i < USER_SIZE; ++i) {
    users[i].state = IDLE;
    users[i].time_till_tx = dist(gen);
    users[i].tx_progress = 0;
  }

  // main loop
  int total_collisions = 0;
  for( int i = 0; i < SIMULATION_TIME*1000; ++i) {
    int users_txing_on_tick = 0;

    // check users loop
    for( int k = 0; k < USER_SIZE; ++k) {
      if( users[k].state == TXING ) {
        ++ users_txing_on_tick;

        // reset state machine if tx progress reached
        if( users[k].tx_progress <= 0) {
          users[k].time_till_tx = dist(gen);
          users[k].state = IDLE;
        }
        else
          users[k].tx_progress --;
      }
      else if( users[k].time_till_tx <= 0 )
        users[k].state = TXING;
      else
        users[k].time_till_tx --;
    }

    if( users_txing_on_tick >= 2)
      total_collisions ++;

    for( int k = 0; k < users_txing_on_tick; ++k) {
      while( collisions.size() <= k )
        collisions.push_back(0);

      collisions[k] ++;
    }
    //// debug stuff
    //if( users_txing_on_tick != 0 )
    //  printf("timestep: %d users: %d\n", i, users_txing_on_tick);
  }

  printf("estimated collisions per second: %f\n", (double) total_collisions/SIMULATION_TIME);
  int i = 0;
  for( auto v : collisions) {
    i++;
    if( i >= 1)
      std::cout<<"Spent "<< (double) v/collisions[0] <<" with "<< i<<" collisions"<< std::endl;
  }

  return 0;
}

