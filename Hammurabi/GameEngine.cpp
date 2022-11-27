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
    cout << "Обнаружена сохранённая игра, хотите продолжить?\nY - Да.\nn - Нет"
         << endl;

    bool inputSuccessfull = false;
    char input = '\0';
    do {
      cout << "Введите ответ: ";
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
    cout << "Мой повелитель, соизволь поведать тебе:\nВ году " << state_.round
         << " твоего высочайшего правления " << endl;

  if (state_.people_dead)
    cout << state_.people_dead << " человек умерли с голоду" << endl;

  if (state_.people_arrived)
    cout << ", и " << state_.people_arrived
         << " человек прибыли в наш великий город;" << endl;

  if (state_.is_plague) cout << "Чума уничтожила половину населения;" << endl;

  cout << "Население города сейчас составляет " << state_.population
       << " человек;" << endl;

  if (state_.wheat_harvested)
    cout << "Мы собрали " << state_.wheat_harvested << " бушелей пшеницы, по "
         << state_.wheat_per_acre << " бушеля с акра;" << endl;

  if (state_.wheat_eaten_by_rats) {
    cout << "Крысы истребили " << state_.wheat_eaten_by_rats
         << " бушелей пшеницы, оставив " << state_.wheat_stash
         << " бушеля в амбарах;" << endl;
  } else {
    cout << "В амбарах находится " << state_.wheat_stash << " бушелей пшеницы;"
         << endl;
  }

  cout << "Город сейчас занимает " << state_.acreage << " акров;" << endl;
  cout << "1 акр земли стоит сейчас " << state_.acre_cost << " бушель." << endl;
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

  cout << "Что пожелаешь, повелитель?" << endl;

  do {
    cout << "Сколько акров земли повелеваешь купить?" << endl;
    acres_to_buy = AcceptInput();

    cout << "Сколько акров земли повелеваешь продать?" << endl;
    acres_to_sell = AcceptInput();

    cout << "Сколько бушелей пшеницы повелеваешь съесть?" << endl;
    wheat_to_eat = AcceptInput();

    cout << "Сколько акров земли повелеваешь засеять?" << endl;
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
      cout << "О, повелитель, пощади нас! У нас только " << state_.population
           << " человек, " << state_.wheat_stash << " бушелей пшеницы и "
           << state_.acreage << " акров земли !" << endl;
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
    cout << "Из-за вашей некомпетентности в управлении, более 45% населения "
            "умерли от голода. Народ устроил бунт и изгнал вас из города."
         << endl;

  if (result == 2)
    cout << "Все умерли, вам больше некем править. Город опустел и пришёл в "
            "упадок. Совесть будет мучать вас до самой смерти."
         << endl;
}
void GameEngine::GameFinished() {
  using std::cout;
  using std::endl;

  int acres_per_person_ratio = state_.population / state_.acreage;
  double average_death_ratio = 0.0;  // TODO: Add death ratio calculation

  if (average_death_ratio > kAverageDeathRateHeigh &&
      acres_per_person_ratio < kAcresPerPersonLow) {
    cout << "Из-за вашей некомпетентности в управлении, народ устроил "
            "бунт, и изгнал вас их города.Теперь вы вынуждены влачить "
            "жалкое существование в изгнании."
         << endl;

    return;
  }

  if (average_death_ratio > kAverageDeathRateMed &&
      acres_per_person_ratio < kAcresPerPersonMed) {
    cout << "Вы правили железной рукой, подобно Нерону и Ивану Грозному. Народ "
            "вздохнул с облегчением, и никто больше не желает видеть вас "
            "правителем."
         << endl;
    return;
  }

  if (average_death_ratio > kAverageDeathRateLow &&
      acres_per_person_ratio < kAcresPerPersonHigh) {
    cout << "Вы справились вполне неплохо, у вас, конечно, есть "
            "недоброжелатели, но многие хотели бы увидеть вас во главе города "
            "снова."
         << endl;
    return;
  }

  cout << "Фантастика! Карл Великий, Дизраэли и Джефферсон вместе не "
          "справились бы лучше!"
       << endl;
}

bool GameEngine::CanLoadState() {
  // TODO: Add save check logic
  return false;
}

void GameEngine::LoadState() {
  std::cout << "Игра успешно загружена." << std::endl;
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
