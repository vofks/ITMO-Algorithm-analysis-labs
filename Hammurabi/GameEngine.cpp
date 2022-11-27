#include "./GameEngine.hpp"

#include <cmath>
#include <iostream>
#include <random>

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

  bool isLoading = false;
  if (CanLoadState()) {
    cout << "���������� ���������� ����, ������ ����������?\nY - ��.\nn - ���"
         << endl;

    bool inputSuccessfull = false;
    char input = '\0';
    do {
      cout << "������� �����: ";
      cin >> input;
      inputSuccessfull = input == 'Y' || input == 'n';

      isLoading = inputSuccessfull && input == 'Y';
    } while (!inputSuccessfull);
  };

  if (isLoading) {
    LoadState();
  }

  while (true) {
    StartRound();

    if (state_.round == 10) {
      GameFinished();
      return;
    }

    int result = Step();

    if (result != 0) {
      GameOver(result);
      return;
    }
  }
}

void GameEngine::StartRound() {
  state_.acre_cost = GetInRangeValue(kMinAcreCost, kMaxAcreCost);
  ShowRoundInfo();

  if (state_.round == 10) {
  }
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
  double wheat_to_spend = 0;
  bool success = true;

  cout << "��� ���������, ����������?" << endl;

  do {
    cout << "������� ����� ����� ����������� ������?" << endl;
    acres_to_buy = AcceptInput();

    cout << "������� ����� ����� ����������� �������?" << endl;
    acres_to_sell = AcceptInput();

    cout << "������� ������� ������� ����������� ������?" << endl;
    wheat_to_eat = AcceptInput();

    cout << "������� ����� ����� ����������� �������?" << endl;
    acres_to_plant = AcceptInput();

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

  double rat_eaten =
      state_.wheat_stash * GetInRangeValue(kMinRatRatio, kMaxRatRatio);
  state_.wheat_eaten_by_rats = rat_eaten;
  state_.wheat_stash -= rat_eaten;

  int death_count = state_.population - wheat_to_eat / kWheatPerPerson;
  if (death_count > state_.population * kMaxDeathRateRatio) {
    return -1;
  }

  if (death_count == state_.population) {
    return -2;
  }

  state_.people_dead = death_count;
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

  return 0;
}

void GameEngine::GameOver(int result) {
  using std::cout;
  using std::endl;

  // TODO: add constants
  if (result == -1)
    cout << "��-�� ����� ���������������� � ����������, ����� 45% ��������� "
            "������ �� ������. ����� ������� ���� � ������ ��� �� ������."
         << endl;

  if (result == 2)
    cout << "��� ������, ��� ������ ����� �������. ����� ������� � ������ � "
            "������. ������� ����� ������ ��� �� ����� ������."
         << endl;
}
void GameEngine::GameFinished() {
  using std::cout;
  using std::endl;

  int acres_per_person_ratio = state_.population / state_.acreage;
  double average_death_ratio = 0.0;  // TODO: Add death ratio calculation

  if (average_death_ratio > kAverageDeathRateHeigh &&
      acres_per_person_ratio < kAcresPerPersonLow) {
    cout << "��-�� ����� ���������������� � ����������, ����� ������� "
            "����, � ������ ��� �� ������.������ �� ��������� ������� "
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
  // TODO: Add save check logic
  return false;
}

void GameEngine::LoadState() {
  std::cout << "���� ������� ���������." << std::endl;
  // TODO: Add load logic
}

void GameEngine::SaveState() {
  // TODO: Add save logic;
}

int GameEngine::AcceptInput() {
  using std::cin;

  int value = -1;
  do {
    cin >> value;
  } while (!(value >= 0));

  return value;
}

int GameEngine::GetInRangeValue(int low, int high) {
  return std::uniform_int_distribution<>(low, high)(gen_);
}

double GameEngine::GetInRangeValue(double low, double high) {
  return std::uniform_real_distribution<>(low, high)(gen_);
}
