#pragma once

typedef struct
{
    double middlegame;
    double endgame;
} Feature;

Feature create_feature(double white_value, double black_value, double game_phase);
double get_feature_score(Feature feature, double middlegame_weight, double endgame_weight);