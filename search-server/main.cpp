// Решите загадку: Сколько чисел от 1 до 1000 содержат как минимум одну цифру 3?
// Напишите ответ здесь:

// Закомитьте изменения и отправьте их в свой репозиторий.
#include <algorithm>
#include <iostream>
#include <set>
#include <map>
#include <string>
#include <utility>
#include <vector>
#include <cmath>

using namespace std;

const int MAX_RESULT_DOCUMENT_COUNT = 5;

string ReadLine() {
    string s;
    getline(cin, s);
    return s;
}

int ReadLineWithNumber() {
    int result = 0;
    cin >> result;
    ReadLine();
    return result;
}

vector<string> SplitIntoWords(const string& text) {
    vector<string> words;
    string word;
    for (const char c : text) {
        if (c == ' ') {
            if (!word.empty()) {
                words.push_back(word);
                word.clear();
            }
        } else {
            word += c;
        }
    }
    if (!word.empty()) {
        words.push_back(word);
    }

    return words;
}
string MinusWordWithoutMinus (const string& word) {
        bool IsMinus=false;
        string str=""s;
        for(char  c : word){
            if(c!='-'){
                str+=c;
            }else{
                IsMinus=true;
            }    
        }
        if(IsMinus){
            return str;
        }else{
            return ""s;
        }
    }
bool IsMinusWord(const string& word){
    if(word[0]=='-'){
        
        return true;
    }else{
        return false;
    }
}

struct Document {
    int id;
    double relevance;
};

class SearchServer {
public:
    void SetStopWords(const string& text) {
        if(!text.empty()){
            for (const string& word : SplitIntoWords(text)) {
                stop_words_.insert(word);
            }
        }
    }

    void AddDocument(int document_id, const string& document) {
        vector<string> words = SplitIntoWordsNoStop(document);
        ++document_count_;
        double k=1/(0.0+words.size());
        //cout<<"size "<<size<<endl;
        for(string & word : words){
           if(word_to_document_freqs_[word].count(document_id)>0){
                //word_to_document_freqs_[word][document_id]=(word_to_document_freqs_[word][document_id]*size+1)/size;
                word_to_document_freqs_[word][document_id]=word_to_document_freqs_[word][document_id]+k;
                //cout<<word_to_document_freqs_[word][document_id]<<endl;
            }else{
                word_to_document_freqs_[word].insert({document_id,k});
                //cout<<word_to_document_freqs_[word][document_id]<<endl;
            }
        }
        //cout<<endl;
    }

    vector<Document> FindTopDocuments(const string& raw_query) const {
        Query query = ParseQuery(raw_query);
        auto matched_documents = FindAllDocuments(query);
        sort(matched_documents.begin(), matched_documents.end(),
             [](const Document& lhs, const Document& rhs) {
                 return lhs.relevance > rhs.relevance;
             });
        if (matched_documents.size() > MAX_RESULT_DOCUMENT_COUNT) {
            matched_documents.resize(MAX_RESULT_DOCUMENT_COUNT);
        }
        return matched_documents;
    }

private:
    int document_count_ = 0;
    struct  Query {
        set<string> query_words_;
        set<string> query_minus_words_;
    };
    map<string, map<int, double>> word_to_document_freqs_;
    set<string> stop_words_;

    bool IsStopWord(const string& word) const {
        return stop_words_.count(word) > 0;
    }

    vector<string> SplitIntoWordsNoStop(const string& text) const {
        vector<string> words;
        for (const string& word : SplitIntoWords(text)) {
            if (!IsStopWord(word)) {
                words.push_back(word);
            }
        }
        return words;
    }

    Query ParseQuery(const string& text) const {
        Query result;
        string str;
        for (const string& word : SplitIntoWordsNoStop(text)) {
            
            //cout<<word<<" ";
            if(!IsMinusWord(word)){
                result.query_words_.insert(word);
                //cout<<word<<" / ";
            }else{
                result.query_minus_words_.insert(word.substr(1,word.size()-1));
                //cout<<word.substr(1,word.size()-1)<<" minus ";
            }
        }
        return result;
    }

    vector<Document> FindAllDocuments(const Query & query_words) const {
        map<int, double> matched_documents;
        double idf=0.0;
        for(const string & qw : query_words.query_words_){
            if(word_to_document_freqs_.count(qw)>0){
                idf=log(document_count_/(0.0+word_to_document_freqs_.at(qw).size()));
                //cout<<"idf "<<idf<<endl;
                for (const auto& [id, tf] : word_to_document_freqs_.at(qw)){
                    if(matched_documents.contains(id)>0){
                        //cout<<tf<<" ";
                        matched_documents[id]+=idf*tf;
                        //cout<<matched_documents[id]<<" ";
                    }else{
                        matched_documents[id]=idf*tf;
                    }
                    //cout<<endl;
                }
            }
        }
        for(const string & qw : query_words.query_minus_words_){
            if(word_to_document_freqs_.count(qw)>0){
                for (const auto& [id, relav] : word_to_document_freqs_.at(qw)){
                    matched_documents.erase(id);
                }
            }
        }
        vector<Document> result;
        for (const auto& [id, relav] : matched_documents){
            result.push_back({id,relav});
        }
        
        return result;
    }
    
};

SearchServer CreateSearchServer() {
    SearchServer search_server;
    search_server.SetStopWords(ReadLine());

    const int document_count = ReadLineWithNumber();
    for (int document_id = 0; document_id < document_count; ++document_id) {
        search_server.AddDocument(document_id, ReadLine());
    }

    return search_server;
}

int main() {
    const SearchServer search_server = CreateSearchServer();

    const string query = ReadLine();
    for (const auto& [document_id, relevance] : search_server.FindTopDocuments(query)) {
        cout << "{ document_id = "s << document_id << ", "
             << "relevance = "s << relevance << " }"s << endl;
    }
}
