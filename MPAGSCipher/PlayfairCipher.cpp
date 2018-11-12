// Standard library includes
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>

// Out project headers
#include "PlayfairCipher.hpp"

PlayfairCipher::PlayfairCipher( const std::string& key )
{
    PlayfairCipher::setKey(key);
}

void PlayfairCipher::setKey( const std::string& key )
{
    // store the original key
    key_ = key;

    // Append the alphabet
    key_ += "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    // Make sure the key is upper case
    std::transform(key_.begin(), key_.end(), key_.begin(), ::toupper);

    // Remove non-alpha characters
    auto func1 = [] (char in_char)
    {
        return(!std::isalpha(in_char));
    };
    auto iter = std::remove_if(key_.begin(), key_.end(), func1);
    key_.erase(iter, key_.end());

    // Change J -> I
    auto func2 = [] (char in_char)
    {
        if(in_char=='J') return('I');
        else return(in_char);
    };
    std::transform(key_.begin(), key_.end(), key_.begin(), func2);

    // Remove duplicated letters
    std::string enc_lett;
    auto func3 = [&] (char in_char)
    {
        if( enc_lett.find(in_char) == std::string::npos)
        {
            enc_lett += in_char;
            return(false);
        }
        else return(true);
    };
    auto iter2 = std::remove_if(key_.begin(), key_.end(), func3);
    key_.erase(iter2, key_.end());

    // Store the coords of each letter

    // Store the playfair chipher key map
}


std::string PlayfairCipher::applyCipher( const std::string& inputText, const CipherMode cipherMode ) const
{

    // Create the output string
    std::string outputText {inputText};

    // Change J -> I
    auto func3 = [] (char in_char)
    {
        if(in_char=='J') return('I');
        else return(in_char);
    };
    std::transform(outputText.begin(), outputText.end(), outputText.begin(), func3);

    // If repeated chars in a digraph add an X or Q if XX
    for( size_t i=1; i<outputText.size(); i+=2)
    {
        if(outputText[i]==outputText[i-1])
        {
            outputText.insert(i, ( (outputText[i]=='X')?("Q"):("X")) );
        }
    }

    // if the size of input is odd, add a trailing Z

    if(outputText.size()%2 != 0)
    {
        outputText.push_back('Z');
    }

    // Loop over the input in Digraphs
    for( size_t i=0; i<outputText.size(); i+=2)
    {
        //   - Find the coords in the grid for each digraph
        int index1 = key_.find(outputText[i]);
        int index2 = key_.find(outputText[i+1]);

        //   - Apply the rules to these coords to get 'new' coords
        if( (index2/5-index1/5)==0 )
        {
            if(cipherMode==CipherMode::Encrypt)
            {
                outputText[i]   = key_[index1+((index1%5==4)?(-4):(1))];
                outputText[i+1] = key_[index2+((index2%5==4)?(-4):(1))];
            }
            else
            {
                outputText[i]   = key_[index1+((index1%5==0)?(4):(-1))];
                outputText[i+1] = key_[index2+((index2%5==0)?(4):(-1))];
            }
        }      
        else if( abs(index2-index1)%5 == 0  )
        {
            if(cipherMode==CipherMode::Encrypt)
            {
                outputText[i]   = key_[index1+((index1>19)?(-20):(5))];
                outputText[i+1] = key_[index2+((index2>19)?(-20):(5))];
            }
            else
            {
                outputText[i]   = key_[index1+((index1<5)?(20):(-5))];
                outputText[i+1] = key_[index2+((index2<5)?(20):(-5))];
            }
        }
        else
        {
            outputText[i]   = key_[index1+(index2%5-index1%5)];
            outputText[i+1] = key_[index2-(index2%5-index1%5)];
        }

        //   - Find the letter associated with the new coords
    }

    // return the text
    return outputText;
}
