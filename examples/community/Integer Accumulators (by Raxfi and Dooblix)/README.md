# Integer Accumulators

The first ever community contribution to Minez comes from [Raxfi](https://github.com/raxfi), who took on the challenge of writing an integer accumulation program. After hearing about it, I (Dooblix) wrote my own version for comparison.

## What does the program do?

Both programs accumulate (sum up) a series of integer values and output the result.

- Dooblix's version: [`accumulator_dooblix.minez`](./accumulator_dooblix.minez)
- Raxfi's version: [`accumulator_raxfi.minez`](./accumulator_raxfi.minez)

## Usage
The input expectation for both programs is the same: integers separated by `\n`. All input integers are accumulated, and the result is output once a zero is entered.

## Comparison

> Note: Dooblix is the creator of Minez, while Raxfi had no prior experience with the language, which explains the difference in development time.

| Category         | Dooblix    | Raxfi      |
|-------------------|-----------|------------|
| Time to write     | ~2 min    | ~30 min    |
| Size              | 37 bytes  | 26 bytes   |
| Memory usage      | 3 + len(list) regs | 3 + len(list) regs |
| Speed             | No measurable difference | No measurable difference |

Despite the lack of prior experience, Raxfi's solution is smaller: a fresh perspective on a language can sometimes lead to more compact solutions than the creator's own.

## Thanks, Raxfi!

This is the first ever community contribution to Minez. Hope there's more to come!