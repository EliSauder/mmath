# mmath
A Matrix Math library I am working on for fun/practice using c++20.

## Features
### Developed/Being Devloped

- [ ] Matricies
  - [x] Creation for any integral/fp type as well as custom types (matching the `expression` concept)
  - [x] Addition, subtraction, multiplication and power (using +, -, \*, ^ respectively)
  - [x] Constant multiplication (using \*) 
  - [x] Trace
  - [x] Transpose
  - [x] Empty Matrix Support   
  - [x] Element access
  - [x] Elementary axis operations (Using operators +, *, >>, <=>)
    - [x] Row/Column literals (_R, _C)
    - [x] Swap axis (<=>)
    - [x] Add axis together (_R/_C + _R/_C >> _R/_C)
    - [x] Multiply axis (\<value\> \* _R/_C >> _R/_C)
    - [x] Add axis with multiplied axis (Combination of above 2)
  - [ ] Determinate
  - [ ] Upper/Lower Triangle matrix creation
  - [ ] Augmented Matricies/Linear systems
  - [ ] R.E.F
  - [ ] R.R.E.F
  - [ ] Inverse
  - [ ] Adjoint
  - [ ] Checks
    - [x] Is square
    - [x] Is identity  
    - [x] Is empty
    - [x] Is zero
    - [x] Equality & Inequalty (== & != between two matricies) 
    - [ ] Is diagonal
    - [ ] Is scalar
    - [ ] Is triangular
    - [ ] Is upper triangular
    - [ ] Is lower triangular
    - [ ] Is symmetric
    - [ ] Is skew symmetric
    - [ ] Is invertible/Is nonsingular
    - [ ] Is homogenous system
  - [ ] Algorithms
    - [ ] Gauss-Jordan reduction

### Being Considered
- [ ] Mathematical Expressions
  - [ ] LaTeX serialization
  - [ ] LaTeX deserialization
- [ ] Tensors
- [ ] Vectors
- [ ] Sets
- [ ] Floating Point Comparison
- [ ] Mathematical Algorithms
  - [ ] Power
  - [ ] Square root
  - [ ] Fast inverse square root
  - [ ] Celiing
  - [ ] Floor
  - [ ] etc...

## Details

### Matrix - Elementary Axis Operations
#### Examples:
```cpp
mmath::matrix_base<int> m{{1, 1, 3}, 
                          {4, 1, 6}, 
                          {7, 1, 9}};

using namespace mmath;
// For any of the below operations, you can replace
// _R with _C to switch to column operations.


auto result1 = m | 1_R <=> 2_R; // Switch row 1 and 2

auto result2 = m | 3 * 1_R >> 1_R // Multiply row 1 by 3

auto result3 = m | 1_R + 2_R >> 2_R; // Add row 1 to row 2 (subtraction can also be used).

auto result4 = m | 3 * 1_R + 2_R >> 2_R; // Multiply row 1 by 3, then add it to row 2.

// All of the above non-swap operations can be paired with <=> to add a swap operation.

auto result2_wswap = m | 3 * 1_R >> 1_R <=> 2_R; // Multiply row 1 by 3, then swap it with row 2.

auto result3_wswap = m | 1_R + 2_R >> 2_R <=> 3_R; // Add row 1 to row 2, then swap row 2 with row 3.

auto result4_wswap = m | 3 * 1_R + 2_R >> 2_R <=> 3_R; // Multiply row 1 by 3, then add it to row 2, then swap with row 3.
```
