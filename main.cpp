#include <iostream>
#include <random>//std::mt_19937
#include <ctime>//std::time
#include <fstream> //std::fstream
#include <string> //std::string
#include <cassert> //assert
#include <algorithm>//find, binary_search
#include <bitset> //std::bitset
#include <set> //std::set
#include <sstream> //std::stringstream

int maxWords() //counts number of words in our text file
{

    std::ifstream file{ "hasla.txt" };


    if (!file)
    {
        std::cerr << "text file failed to open";
        return -1;
    }

    int counter{};
    std::string x{};
    while(std::getline(file, x)) ++counter;

    return counter;
}


int getRandom(int min = 1, int max = maxWords())
{
    //initiate random number generator using std::time
    static std::mt19937 mt{static_cast<unsigned int>(std::time(NULL))};

    //set a range we pick random numbers from
    std::uniform_int_distribution x(min,max);

    return x(mt);
}

std::string getWord(int index = getRandom()) //picks a word from textfile
{
    assert ( index <= maxWords() );

    std::ifstream file{"hasla.txt"};
    std::string temp{};
    for (int i{} ; i < index ; ++i)
        std::getline(file, temp);

    return temp;
}
char getLetter() //asks player for a letter
{

    std::cout << "Enter a letter: ";
    char temp{};
    do
    {
        std::cin >> temp;
    }
    while ( !std::isalpha(temp));

    return temp;
}


void printAnswer(const std::string& answer, const std::bitset<25>& bits)
{
    for (size_t i{0}; i < answer.length(); ++i)
    {
        if( bits.test(i) )
            std::cout << answer[i] << ' ';
        else std::cout << "_ ";
    }
}

void printHangman(int stage)
{

    //convert int to char
    std::stringstream os;
    os << stage;
    char charStage{};
    os >> charStage;

    std::string filename {"wisielec_#.txt"};

    //replace the # with correct index
    for (size_t i{}; i < filename.length(); ++i)
    {
        if (filename[i] == '#')
            filename[i] = charStage;
    }

    //print the textfile
    std::ifstream file(filename);
    std::string str{};
    while(std::getline(file, str))
    {
        std::cout << str << '\n';
    }

    std::cout << '\n';
}


void playGame()
{
    //GAME SETUP

    std::string word{getWord()};
    std::transform(word.begin(), word.end(), word.begin(), ::toupper);
    std::bitset<30> wordBits{}; //mark corresponding word indexes as true if player guesses correctly
    int hangmanStage{1};
    constexpr int FINAL_STAGE{7};
    char playerGuess{};
    //use std::set for alphabetical order
    std::set<char> usedLetters{};

    //set any spaces in the word as found
    for(size_t i{}; i < word.length(); ++i)
    {
        if ( word[i] == ' ')
            wordBits.set(i);
    }

    //GAME LOOP
    while ( true )
    {

        //print game state

        printHangman(hangmanStage);

        for( char c : usedLetters )
            std::cout << c << ' ';
        std::cout << '\n';

        for(size_t i{}; i<word.length(); ++i)
        {
            if(wordBits.test(i))
                std::cout << word[i] << ' ';
            else
                std::cout << '_' << ' ';
        }


        //ask player for a letter

        do
        {
            playerGuess = std::toupper(getLetter());
        }
        //ask again if letter was already used
        while( std::binary_search(usedLetters.begin(), usedLetters.end(), playerGuess ) );
        usedLetters.insert(playerGuess);

        //check if word contains the letter
        auto found{std::find(word.begin(), word.end(), playerGuess)};

        if (found == word.end()) // didnt find a letter
        {
            ++hangmanStage;
        }
        else
        {
            //mark all spots with found letter
            for(size_t i{}; i < word.length(); ++i)
            {
                if ( word[i] == *found )
                    wordBits.set(i);
            }
        }

        if( wordBits.count() == word.length() )//check if user guessed all letters
        {
            std::cout << "You have guessed the word " << word << '\n';
            return;
        }
        if( hangmanStage == FINAL_STAGE )
        {
            system("cls");
            printHangman(FINAL_STAGE);
            std::cout << "The correct answer was " << word << '\n';
            return;
        }
        system("cls");
    }
}

int main()
{
    char playAgain{'y'};
    while(playAgain == 'Y' || playAgain == 'y')
    {
        system("cls");
        playGame();
        std::cout << "Play again? Y/N\n";
        std::cin >> playAgain;
    }


    std::cout << "See you next time\n";


}
