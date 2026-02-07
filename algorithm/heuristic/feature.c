#include "feature.h"

Feature create_feature(double white_value, double black_value, double game_phase)
{
    return (Feature){(white_value - black_value) * (1 - game_phase), (white_value - black_value) * game_phase};
}

double get_feature_score(Feature feature, double middlegame_weight, double endgame_weight)
{
    return feature.middlegame * middlegame_weight + feature.endgame * endgame_weight;
}