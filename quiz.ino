// -----------------------------------------------
// LCD 16x2 HD44780-40
// -----------------------------------------------

#include <LiquidCrystal.h>
#include <Keypad.h>

int D7_pin = 4;
int D6_pin = 5;
int D5_pin = 6;
int D4_pin = 7;
int EN_pin = 11;
int RS_pin = 12;

LiquidCrystal lcd(RS_pin, EN_pin, D4_pin, D5_pin, D6_pin, D7_pin);

const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
char keys[ROWS][COLS] = {
    {'1', '2', '3'},
    {'4', '5', '6'},
    {'7', '8', '9'},
    {'*', '0', '#'}};
byte rowPins[ROWS] = {0, 1, 2, 3}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {10, 9, 8};   //connect to the column pinouts of the keypad

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

const int questions_num = 9;

char *questions[] = {"Which covers most of the Earth's surface", "Which is a mammal", "Frog tongue is",
                     "Which word looks the same in mirror", "Which is the largest number", "Which absorbs digested food",
                     "C can be used on", "The maximun value of integer is", "What is the largest country"};

char *answers[][4] = {{"Sand", "Water", "Cities", "Mountains"},
                      {"Rabbit", "Spider", "Butterfly", "Sparrow"},
                      {"Dry", "Hard", "Sticky", "Colorful"},
                      {"HIM", "POT", "MOM", "SUN"},
                      {"989", "898", "999", "99.9"},
                      {"Blood", "Small Intestine", "Lever", "Stomach"},
                      {"Windows", "Linux", "DOS", "All"},
                      {"-32767", "32767", "-1.7014e+38", "1.7014e+38"},
                      {"Russia", "America", "China", "Canada"}};

char correct[] = {'2', '1', '3', '3', '3', '2', '4', '2', '1'};

int correct_answers = 0;

char get_key_pressed()
{
  char key = '.';
  while (!key || key == '.')
  {
    key = keypad.getKey();
  }
  return key;
}

int get_random_num()
{
  return random(questions_num);
}

void print_question(int question_num)
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(questions[question_num]);
  lcd.setCursor(0, 3);
  lcd.print("0: to show answers");
}

void print_answers(int question_num)
{
  lcd.clear();
  for (int i = 0; i < 4; i++)
  {
    lcd.setCursor(0, i);
    lcd.print(i + 1);
    lcd.setCursor(3, i);
    lcd.print(answers[question_num][i]);
  }
}

void check_answer(char key, int question_num)
{
  lcd.clear();
  lcd.setCursor(0, 0);
  if (key == correct[question_num])
  {
    correct_answers++;
    lcd.print("Yeah! Correct answer");
  }
  else
  {
    lcd.print("Ops! Wrong answer");
  }
  delay(2000);
}

void exit_quiz()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Bye! closing quiz");
  lcd.setCursor(0, 2);
  lcd.print("Score: ");
  lcd.setCursor(7, 2);
  lcd.print(correct_answers);
  lcd.setCursor(8, 2);
  lcd.print("/4");
}

void setup()
{
  lcd.begin(20, 4);
  randomSeed(analogRead(0));
}

void loop()
{
  correct_answers = 0;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Welcome to quiz");
  lcd.setCursor(0, 1);
  lcd.print("0: to start");
  lcd.setCursor(0, 2);
  lcd.print("#: to end or skip");

  char key = get_key_pressed();
  if (key == '0')
  {
    // start the quiz of 4 questions
    for (int i = 0; i < 4; i++)
    {
      int random_num = get_random_num();
      if (random_num < questions_num)
      {
        print_question(random_num);
        key = get_key_pressed();
        while (key != '0' && key != '#')
        {
          key = get_key_pressed();
        }
        if (key == '0')
        {
          // go to answers
          print_answers(random_num);
          key = get_key_pressed();
          while (key != '1' && key != '2' && key != '3' && key != '4' && key != '#')
          {
            key = get_key_pressed();
          }
          if (key != '#')
          {
            // answer the question
            check_answer(key, random_num);
          }
          else
          {
            // exit questions
            exit_quiz();
          }
        }
        else
        {
          // exit questions
          exit_quiz();
        }
      }
    }
    exit_quiz();
  }

  delay(3000);
}
