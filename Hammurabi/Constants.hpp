#ifndef _CONSTANTS_HPP_

#define _CONSTANTS_HPP_

// Game logic constants
//
// Start values
const int kStartPopulation = 100;
const int kStartAcreage = 1000;
const int kStartWheatStash = 2800;

// Limits
const int kMinAcreCost = 17;
const int kMaxAcreCost = 26;
const int kWheatPerPerson = 20;
const int kMaxCultivatedArea = 10;
const double kSeedsPerAcre = 0.5;
const int kMinWheatPerAcre = 1;
const int kMaxWheatPerAcre = 6;
const double kMinRatRatio = 0;
const double kMaxRatRatio = 0.07;
const double kMaxDeathRateRatio = 0.45;
const double kPlagueProbability = 0.15;
const double kPlagueDeathRatio = 0.5;

// Game stats constants
const double kAverageDeathRateLow = 0.03;
const double kAverageDeathRateMed = 0.1;
const double kAverageDeathRateHeigh = 0.33;

const int kAcresPerPersonLow = 7;
const int kAcresPerPersonMed = 9;
const int kAcresPerPersonHigh = 10;

#endif  // _CONSTANTS_HPP_
