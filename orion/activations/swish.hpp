//
// Created by RKuang on 9/22/2022.
//

#ifndef ORION_SWISH_HPP
#define ORION_SWISH_HPP

#include "orion/orion_types.hpp"
#include "sigmoid.hpp"

namespace orion
{
// TODO: prone to exploding gradients, need to find a way to clip in optimizers
// https://arxiv.org/pdf/1710.05941.pdf
class Swish
{
public:
    static Tensor<2> Activate(const Tensor<2> &features)
    {
        // a = z/(1 + e^-z) = z * sigmoid(z)
        return features * features.sigmoid();
    }


    static Tensor<3> Activate(const Tensor<3> &features)
    {
        return features * features.sigmoid();
    }


    static Tensor<2> Gradients(const Tensor<2> &features)
    {
        auto one = static_cast<Scalar>(1.0);

        Tensor<2> swish = features * features.sigmoid();
        return swish + features.sigmoid() * (one - swish);
    }


    static Tensor<3> Gradients(const Tensor<3> &features)
    {
        auto one = static_cast<Scalar>(1.0);

        Tensor<3> swish = features * features.sigmoid();
        return swish + features.sigmoid() * (one - swish);
    }
};

}

#endif //ORION_SWISH_HPP