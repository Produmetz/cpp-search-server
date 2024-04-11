#pragma once
#include <algorithm>
#include <stdexcept>
#include <map>
#include <set>
#include <vector>
#include <numeric>
#include "string_processing.h"
#include "document.h"


using namespace std::string_literals;

class SearchServer {
public:
    template <typename StringContainer>
    explicit SearchServer(const StringContainer& stop_words);

    explicit SearchServer(const std::string & stop_words_text)
        : SearchServer(
            SplitIntoWords(stop_words_text)) 
    {
        
    }
    void AddDocument(int document_id, const std:: string & document, DocumentStatus status,const std::vector<int>& ratings);

    template <typename DocumentPredicate>
    std::vector<Document> FindTopDocuments(const std::string& raw_query, DocumentPredicate document_predicate) const ;

    std::vector<Document> FindTopDocuments(const std::string& raw_query, DocumentStatus status) const ;
    std::vector<Document> FindTopDocuments(const std::string& raw_query) const ;

    int GetDocumentCount() const ;

    std::tuple<std::vector<std::string>, DocumentStatus> MatchDocument(const std::string& raw_query, int document_id) const ;

    int GetDocumentId(int index) const ;

private:
    struct DocumentData {
        int rating;
        DocumentStatus status;
    };
    const std::set<std::string> stop_words_;
    std::map<std::string, std::map<int, double>> word_to_document_freqs_;
    std::map<int, DocumentData> documents_ ;
    std::vector<int> ordered_id_;

    bool IsStopWord(const std::string& word) const ;
    
    std::vector<std::string> SplitIntoWordsNoStop(const std::string& text) const ;

    int ComputeAverageRating(const std::vector<int>& ratings) const;

    struct QueryWord {
        std::string data;
        bool is_minus;
        bool is_stop;
    };

    QueryWord ParseQueryWord(std::string text) const ;

    struct Query {
        std::set<std::string> plus_words;
        std::set<std::string> minus_words;
        bool with_stop_word;
    };

    Query ParseQuery(const std::string& text) const ;
    
    double ComputeWordInverseDocumentFreq(const std::string & word) const ;

    template <typename DocumentPredicate>
    std::vector<Document> FindAllDocuments(
        const Query& query,
        DocumentPredicate document_predicate) const ;
};