// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "HiddenWordList.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();

    Isograms = GetValidWords(Words);
    
    SetupGame();
}

void UBullCowCartridge::OnInput(const FString &PlayerInput)
{
    ClearScreen();

    if (bGameOver == true)
    {
        ClearScreen();
        SetupGame();
    }
    else
    {
        ProcessGuess(PlayerInput);
    }
}

void UBullCowCartridge::SetupGame()
{
    PrintLine(TEXT("Welcome to the Bull Cow Game!"));

    HiddenWord = Isograms[FMath::RandRange(0, Isograms.Num() - 1)];
    Lives = HiddenWord.Len();
    bGameOver = false;

    PrintLine(TEXT("Guess the %i letter word!"), HiddenWord.Len());
    PrintLine(TEXT("You have %i lives!"), Lives);
    PrintLine(TEXT("Type in your guess. \nPress enter to continue..."));
    // PrintLine(TEXT("The HiddenWord is: %s."), *HiddenWord); // Debug line
}

void UBullCowCartridge::EndGame()
{
    bGameOver = true;
    PrintLine(TEXT("The hidden word was: %s"), *HiddenWord);
    PrintLine(TEXT("\nPress enter to play again."));
}

void UBullCowCartridge::ProcessGuess(const FString &Guess)
{
    if (Guess == HiddenWord)
    {
        PrintLine(TEXT("Good job! You got it!"));
        EndGame();
        return;
    }
    
    if (Lives > 1  && Guess.Len() != HiddenWord.Len() && !IsIsogram(Guess))
    {
        PrintLine(TEXT("The hidden word is %i letters long."), HiddenWord.Len());
        PrintLine(TEXT("No repeating letters. \nTry again!"));
        PrintLine(TEXT("You have %i lives left!"), Lives);
        return;
    }

    if (Lives > 1  && Guess.Len() != HiddenWord.Len())
    {
        PrintLine(TEXT("The hidden word is %i letters long. \nTry again!"), HiddenWord.Len());
        PrintLine(TEXT("You have %i lives left!"), Lives);
        return;
    }

    if (!IsIsogram(Guess))
    {
        PrintLine(TEXT("No repeating letters. \nTry again!"));
        PrintLine(TEXT("You have %i lives left!"), Lives);
        return;
    }
    
    if (Lives == 1)
    {
        --Lives;
        PrintLine(TEXT("Oh no! You've lost all your lives."));
        EndGame();
        return;
    }

    FBullCowCount Score = GetBullCows(Guess);

    PrintLine(TEXT("You have %i Bulls and %i Cows"), Score.Bulls, Score.Cows);

    PrintLine(TEXT("Your guess is incorrect. \nTry again!"));
    PrintLine(TEXT("You have %i lives left!"), --Lives);
}

bool UBullCowCartridge::IsIsogram(const FString &Word) const
{
    for (int32 Index = 0; Index < (Word.Len() - 1); Index++)
    {
        for (int32 Comparison = Index + 1; Comparison < Word.Len(); Comparison++)
        {
            if (Word[Index] == Word[Comparison])
            {
                return false;
            }
        }
    }
    return true;
}

TArray<FString> UBullCowCartridge::GetValidWords(const TArray<FString> &WordList) const
{
    TArray<FString> ValidWords;

    for (FString Word : WordList)
    {
        if (Word.Len() >= 4 && Word.Len() <= 8 && IsIsogram(Word))
        {
            ValidWords.Emplace(Word);
        }
    }
    return ValidWords;
}

FBullCowCount UBullCowCartridge::GetBullCows(const FString &Guess) const
{
    FBullCowCount Count;

    for (int32 GuessIndex = 0; GuessIndex < Guess.Len(); GuessIndex++)
    {
        if (Guess[GuessIndex] == HiddenWord[GuessIndex])
        {
            Count.Bulls++;
            continue;
        }

        for (int32 HiddenIndex = 0; HiddenIndex < HiddenWord.Len(); HiddenIndex++)
        {
            if (Guess[GuessIndex] == HiddenWord[HiddenIndex])
            {
                Count.Cows++;
                break;
            }
        }
    }
    return Count;
}