#include "./GameEngine.hpp"

#include <cmath>
#include <fstream>
#include <iostream>
#include <numeric>
#include <random>
#include <vector>

#include "./StateSerializer.hpp"

using hammurabi::GameEngine;

GameEngine::GameEngine() {
  auto seed = std::random_device()();

  state_ = GameState();
  state_.seed = seed;

  gen_ = std::mt19937(seed);
}

void GameEngine::MainLoop() {
  using std::cin;
  using std::cout;
  using std::endl;

  bool load_game = false;
  if (CanLoadState()) {
    cout << "���������� ���������� ����, ������ ����������?\nY - ��.\nn - ���"
         << endl;

    load_game = AcceptYesNoInput();
  };

  if (load_game) {
    LoadState();
  }

  while (true) {
    StartRound();

    if (state_.round == kLastRoundNumber) {
      GameFinished();
      return;
    }

    int result = Step();

    switch (result) {
      case kSaveAndExit: {
        SaveState();
        std::cout << "���� ���������." << std::endl;

        return;
      }
      case kAllDead:
      case kStarvationRateExceeded: {
        GameOver(result);

        return;
      }
      default: {
        break;
      }
    }
  }
}

void GameEngine::StartRound() {
  state_.acre_cost = GetInRangeValue(kMinAcreCost, kMaxAcreCost);

  ShowRoundInfo();
}

void GameEngine::ShowRoundInfo() {
  using std::cout;
  using std::endl;

  if (state_.round)
    cout << "��� ����������, �������� �������� ����:\n� ���� " << state_.round
         << " ������ ����������� ��������� " << endl;

  if (state_.people_dead)
    cout << state_.people_dead << " ������� ������ � ������" << endl;

  if (state_.people_arrived)
    cout << ", � " << state_.people_arrived
         << " ������� ������� � ��� ������� �����;" << endl;

  if (state_.is_plague) cout << "���� ���������� �������� ���������;" << endl;

  cout << "��������� ������ ������ ���������� " << state_.population
       << " �������;" << endl;

  if (state_.wheat_harvested)
    cout << "�� ������� " << state_.wheat_harvested << " ������� �������, �� "
         << state_.wheat_per_acre << " ������ � ����;" << endl;

  if (state_.wheat_eaten_by_rats) {
    cout << "����� ��������� " << state_.wheat_eaten_by_rats
         << " ������� �������, ������� " << state_.wheat_stash
         << " ������ � �������;" << endl;
  } else {
    cout << "� ������� ��������� " << state_.wheat_stash << " ������� �������;"
         << endl;
  }

  cout << "����� ������ �������� " << state_.acreage << " �����;" << endl;
  cout << "1 ��� ����� ����� ������ " << state_.acre_cost << " ������." << endl;
}

int GameEngine::Step() {
  using std::cin;
  using std::cout;
  using std::endl;

  int acres_to_buy = 0;
  int acres_to_sell = 0;
  int wheat_to_eat = 0;
  int acres_to_plant = 0;
  int new_acreage = 0;
  int wheat_to_spend = 0;
  bool success = true;

  cout << "��� ���������, ����������?" << endl;
  cout << "1 - ����������" << endl;
  cout << "2 - ��������� � �����" << endl;

  int command = AcceptIntegerInput(1, 2);
  if (command == 2) {
    return kSaveAndExit;
  }

  do {
    cout << "������� ����� ����� ����������� ������?" << endl;
    acres_to_buy = AcceptIntegerInput();

    cout << "������� ����� ����� ����������� �������?" << endl;
    acres_to_sell = AcceptIntegerInput();

    cout << "������� ������� ������� ����������� ������?" << endl;
    wheat_to_eat = AcceptIntegerInput();

    cout << "������� ����� ����� ����������� �������?" << endl;
    acres_to_plant = AcceptIntegerInput();

    int acre_cost = state_.acre_cost;
    wheat_to_spend = acres_to_buy * acre_cost - acres_to_sell * acre_cost +
                     wheat_to_eat +
                     static_cast<double>(acres_to_plant) * kSeedsPerAcre;

    new_acreage = state_.acreage + acres_to_buy - acres_to_sell;

    success = state_.wheat_stash >= wheat_to_spend && new_acreage >= 0 &&
              acres_to_plant <= new_acreage &&
              acres_to_plant <= state_.population * kMaxCultivatedArea;

    if (!success) {
      cout << "�, ����������, ������ ���! � ��� ������ " << state_.population
           << " �������, " << state_.wheat_stash << " ������� ������� � "
           << state_.acreage << " ����� ����� !" << endl;
    }

  } while (!success);

  state_.wheat_per_acre = GetInRangeValue(kMinWheatPerAcre, kMaxWheatPerAcre);
  state_.wheat_stash -= wheat_to_spend;
  state_.acreage = new_acreage;
  state_.acres_planted = acres_to_plant;

  int wheat_harvested = state_.acres_planted * state_.wheat_per_acre;
  state_.wheat_harvested = wheat_harvested;
  state_.wheat_stash += wheat_harvested;

  int rat_eaten =
      state_.wheat_stash * GetInRangeValue(kMinRatRatio, kMaxRatRatio);
  state_.wheat_eaten_by_rats = rat_eaten;
  state_.wheat_stash -= rat_eaten;

  int death_count = state_.population - wheat_to_eat / kWheatPerPerson;
  if (death_count > state_.population * kMaxDeathRateRatio) {
    return kStarvationRateExceeded;
  }

  if (death_count == state_.population) {
    return kAllDead;
  }

  state_.people_dead = death_count;
  state_.cumulative_starvation_death_per_year += death_count;
  state_.population -= death_count;

  int people_arrived = death_count / 2 +
                       (5 - state_.wheat_per_acre) * state_.wheat_stash / 600 +
                       1;
  if (people_arrived < 0) people_arrived = 0;
  if (people_arrived > 50) people_arrived = 50;
  state_.people_arrived = people_arrived;
  state_.population += people_arrived;

  if (GetInRangeValue(0.0, 1.0) < kPlagueProbability) {
    state_.is_plague = true;
    state_.population = floor(state_.population * kPlagueDeathRatio);
  }

  state_.round += 1;

  return kInProgress;
}

void GameEngine::GameOver(int result) {
  using std::cout;
  using std::endl;

  if (result == kStarvationRateExceeded)
    cout << "��-�� ����� ���������������� � ����������, ����� 45% ��������� "
            "������ �� ������. ����� ������� ���� � ������ ��� �� ������."
         << endl;

  if (result == kAllDead)
    cout << "��� ������, ��� ������ ����� �������. ����� ������� � ������ � "
            "������. ������� ����� ������ ��� �� ����� ������."
         << endl;
}
void GameEngine::GameFinished() {
  using std::accumulate;
  using std::cout;
  using std::endl;

  int acres_per_person_ratio = state_.population / state_.acreage;

  double average_death_ratio =
      double(state_.cumulative_starvation_death_per_year) / state_.round;

  if (average_death_ratio > kAverageDeathRateHeigh &&
      acres_per_person_ratio < kAcresPerPersonLow) {
    cout << "��-�� ����� ���������������� � ����������, ����� ������� "
            "����, � ������ ��� �� ������. ������ �� ��������� ������� "
            "������ ������������� � ��������."
         << endl;

    return;
  }

  if (average_death_ratio > kAverageDeathRateMed &&
      acres_per_person_ratio < kAcresPerPersonMed) {
    cout << "�� ������� �������� �����, ������� ������ � ����� ��������. ����� "
            "�������� � �����������, � ����� ������ �� ������ ������ ��� "
            "����������."
         << endl;
    return;
  }

  if (average_death_ratio > kAverageDeathRateLow &&
      acres_per_person_ratio < kAcresPerPersonHigh) {
    cout << "�� ���������� ������ �������, � ���, �������, ���� "
            "���������������, �� ������ ������ �� ������� ��� �� ����� ������ "
            "�����."
         << endl;
    return;
  }

  cout << "����������! ���� �������, �������� � ���������� ������ �� "
          "���������� �� �����!"
       << endl;
}

bool GameEngine::CanLoadState() {
  // NOTE: Unstable solution.
  return (std::ifstream(kDefaultFileName)).good();
}

void GameEngine::LoadState() { state_ = serializer::DeserializeState(); }

void GameEngine::SaveState() { serializer::SerializeState(state_); }

int GameEngine::AcceptIntegerInput(int min, int max) {
  using std::cin;

  int value = -1;
  do {
    cin >> value;
  } while (!(value >= min && value <= max));

  return value;
}

int GameEngine::GetInRangeValue(int low, int high) {
  return std::uniform_int_distribution<>(low, high)(gen_);
}

double GameEngine::GetInRangeValue(double low, double high) {
  return std::uniform_real_distribution<>(low, high)(gen_);
}

bool GameEngine::AcceptYesNoInput() {
  using std::cin;
  using std::cout;
  using std::endl;

  bool inputSuccessfull = false;
  char input = '\0';
  do {
    cout << "������� �����: ";
    cin >> input;
    inputSuccessfull = input == 'Y' || input == 'n';
  } while (!inputSuccessfull);

  return input == 'Y';
}
