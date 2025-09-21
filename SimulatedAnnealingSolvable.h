//
// Created by david on 9/21/25.
//

#ifndef HALAL_SIMULATEDANNEALINGSOLVABLE_H
#define HALAL_SIMULATEDANNEALINGSOLVABLE_H

template<typename Numeric>
class SimulatedAnnealingSolvable {
protected:
    Numeric temp0;

public:
    virtual float BoltzmannScheduleTemperature(int) = 0;
};

#endif //HALAL_SIMULATEDANNEALINGSOLVABLE_H