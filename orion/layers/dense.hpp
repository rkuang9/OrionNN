//
// Created by macross on 8/7/22.
//

#ifndef ORION_DENSE_HPP
#define ORION_DENSE_HPP

#include "layer.hpp"

namespace orion
{

template<typename Activation>
class Dense : public Layer
{
public:
    explicit Dense(int inputs, int outputs, bool use_bias,
                   const Initializer<2> &initializer = GlorotUniform<2>());

    ~Dense() override = default;

    /**
     * Forward propagation, computes "z" and "a" with the formulas
     * z = wx + b, a = g(z) where g is the chosen Activation template type
     *
     * @param tensor   a rank 2 tensor training sample
     */
    void Forward(const Tensor<2> &input) override;


    /**
     * Forward propagation for hidden layers, passes the previous layer's
     * output as input for Forward(input)
     *
     * @param prev   a reference to the previous layer
     */
    void Forward(const Layer &prev) override;


    /**
     * Backward propagation for hidden layers, calculates
     * loss gradients w.r.t. Z
     *
     * @param next   a reference to the next layer
     */
    void Backward(const Layer &next) override;


    /**
     * Backward propagation as the output layer, calculates
     * loss gradients w.r.t. Z using the loss gradients recorded
     * in the loss object
     *
     * @param loss_function   a reference to the loss object
     */
    void Backward(const LossFunction &loss_function) override;


    /**
     * Updates the layer's weights and bias with dL/dw, dL/db using
     * the provided optimizer
     *
     * @param optimizer   a reference to the optimizer object
     */
    void Update(Optimizer &optimizer) override;


    // getter setters


    /**
     * @return   layer's activation values
     */
    const Tensor<2> &GetOutput2D() const override;


    /**
     * @return   layer's loss gradients w.r.t. pre-activated output (dL / dZ))
     */
    Tensor<2> GetInputGradients2D() const override;


    /**
     * @return   layer's weights
     */
    const Tensor<2> &GetWeights() const override;


    /**
     * @return   loss gradients w.r.t. weights (dL / dw)
     */
    const Tensor<2> &GetWeightGradients() const override;


    /**
     * Set layer's weights
     *
     * @param weights   custom weights with dimensions [output units, input units]
     */
    void SetWeights(const Tensor<2> &weights) override;


    /**
     * @return   layer's bias
     */
    const Tensor<2> &GetBias() const override;


    /**
     * Set layer's bias
     *
     * @param bias   custom bias with dimensions [output units, 1]
     */
    void SetBias(const Tensor<2> &bias) override;


    /**
     * @return   expected rank of forward propagation's input tensor
     */
    int GetInputRank() const override;


    /**
     * @return   expected rank of forward propagation's output tensor
     */
    int GetOutputRank() const override;

private:
    /**
     * Backward propagation, computes the loss gradients
     * w.r.t. weights and bias (dL / dw, dL / db)
     *
     * dL / dZ is calculated in the other Backward() function overrides
     */
    void Backward();

    // backpropagation is a bit different than most activation functions
    EIGEN_STRONG_INLINE void BackwardSoftmax(const Tensor<2> &gradients);

    bool use_bias = true;

    Tensor<2> X; // layer input matrix, stacked column-wise
    Tensor<2> Z; // weighted input matrix, Z = w*X + b
    Tensor<2> A; // activated input matrix after applying g(Z)
    Tensor<2> dL_dZ; // gradients of layer output, received from next layer
    Tensor<2> dL_dX; // input gradients, passed to previous layer as dL_dZ

    Tensor<2> w; // weights matrix
    Tensor<2> b; // bias vector
    Tensor<2> dL_dw; // loss gradients w.r.t. weights
    Tensor<2> dL_db; // loss gradients w.r.t. bias

};

} // namespace orion

#include "dense.ipp"

#endif //ORION_DENSE_HPP
