#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <stdio.h>
#include <fstream>

#include "Ngram.h"

using namespace std;

double GetBigramProb(Vocab &Voc, Ngram &lm, const char *W1, const char *W2){
	// Get index
	VocabIndex W1ID = Voc.getIndex(W1);
	VocabIndex W2ID= Voc.getIndex(W2);

	// OOV
	if(W1ID == Vocab_None){
		W1ID = Voc.getIndex(Vocab_Unknown);
	}
	if(W2ID == Vocab_None){
		W2ID = Voc.getIndex(Vocab_Unknown);
	}

	// Get probability
	VocabIndex Context[] = {W1ID, Vocab_None};
    return lm.wordProb(W2ID, Context);
}

int main(int argc, char *argv[]){
	if(argc != 4){
		cout << "Format: ./mydisambig [language model] [mapping file] [test file]" << endl;
		cout << "ex: ./mydisambig bigram.lm ZhuYin-Big5.map testdata/1.txt > result1.txt" << endl;
		return 0;
	}

	// Prepare
	map<string, vector<string>> Map;
    Vocab Voc;
    Ngram lm(Voc, 2);

    // Read language model
    File File_LM(argv[1], "r"); 
    lm.read(File_LM); 

    // Read Map
    fstream File_Map;
    File_Map.open(argv[2]);
    for(string Line; getline(File_Map, Line);){
    	int len = Line.size();
    	string zhuyin = Line.substr(0, 2);
    	for(int i = 2; i < len; i++){
    		if(Line[i] != ' ' && Line[i] != '\n'){
    			Map[zhuyin].push_back(Line.substr(i, 2));
    			i += 2;
    		}
    	}
	}
	File_Map.close();

	// Open test file and run viterbi
	fstream TestFile;
	TestFile.open(argv[3]);
	// For every test case in test data
	for(string Line; getline(TestFile, Line);){
		// Save all words in LineVocab without spaces
		vector<string> LineVocab;
		int len = Line.size();
		for(int i = 0; i < len; i++){
			if(Line[i] != ' ' && Line[i] != '\n'){
				LineVocab.push_back(Line.substr(i, 2));
				i += 2;
			}
		}

		int Len = LineVocab.size();

		int max_Strlen = 128, max_size = 1024;
		double Delta[max_Strlen][max_size];	//Probability
		int Phi[max_Strlen][max_size];		//Back track id
		// Set everything to 0
		for(int i = 0; i < max_Strlen; i++){
			for(int j = 0; j < max_size; j++){
				Delta[i][j] = 0;
				Phi[i][j] = 0;
			}
		}

		// Initialize
		string FirstWord = LineVocab[0];
		int FirstSize = Map[FirstWord].size();
		for(int i = 0; i < FirstSize; i ++){
			double Probability = GetBigramProb(Voc, lm, Vocab_SentStart, Map[FirstWord][i].c_str());
			Delta[0][i] = Probability;
		}

		// Run Viterbi
		for(int i = 1; i < Len; i++){
			string PreWord = LineVocab[i-1];
			int PreSize = Map[PreWord].size();
			string CurWord = LineVocab[i];
			int CurSize = Map[CurWord].size();
			for(int j = 0; j < CurSize; j++){
				// Iterate through all pre words
				// choose the one with (the highest bigram probability) * Delta
				double BestProb = -999.9;
				for(int k = 0; k < PreSize; k++){
					double CurProb = GetBigramProb(Voc, lm, Map[PreWord][k].c_str(), Map[CurWord][j].c_str());
					double CurDelta = CurProb + Delta[i-1][k];
					if(CurDelta > BestProb){
						BestProb = CurDelta;
						Phi[i][j] = k;
					}
				}
				Delta[i][j] = BestProb;
			}
		}

		// Get last word to </s> probability
		// And choose the best path with highest probability
		double BestProb = -999.9;
		string PreWord = LineVocab[Len-1];
		int PreSize = Map[PreWord].size();
		for(int i = 0; i < PreSize; i++){
			double CurProb = GetBigramProb(Voc, lm, Map[PreWord][i].c_str(), Vocab_SentEnd);
			double CurDelta = CurProb + Delta[Len-1][i];
			if(CurDelta > BestProb){
				BestProb = CurDelta;
				Phi[Len][0] = i;
			}
		}
		Delta[Len][0] = BestProb;

		// Back track and reconstruct the sentence
		vector<string> ReconstrctSentence;
		int BackTrackID = 0;
		for(int i = Len; i > 0; i--){
			BackTrackID = Phi[i][BackTrackID];
			string CurWord = LineVocab[i-1];
			ReconstrctSentence.insert(ReconstrctSentence.begin(), Map[CurWord][BackTrackID]);
		}

		// Print Result
		cout << "<s>";
		for (int i = 0; i < Len; i++)
			cout << ' ' << ReconstrctSentence[i];
		cout << " </s>" << endl;
	}

	// Always remember to close the file pointer!!!
	TestFile.close();

	return 0;
}