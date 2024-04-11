#pragma once
#include "search_server.h"
#include "document.h"
#include <vector>
#include <string>
#include <deque>

using namespace std::string_literals;

class RequestQueue {
public:
    explicit RequestQueue(const SearchServer & search_server);
    
    template <typename DocumentPredicate>
    std::vector<Document> AddFindRequest(const std::string & raw_query, DocumentPredicate document_predicate);
    std::vector<Document> AddFindRequest(const std::string& raw_query, DocumentStatus status);
    std::vector<Document> AddFindRequest(const std::string& raw_query);
    int GetNoResultRequests() const;
private:
    struct QueryResult {
        QueryResult() ;
        QueryResult(std::string  query_words_, int count_documents_in_result_);
        
        std::string query_words = ""s;
        int count_documents_in_result = 0;
    };
    int count_no_result_requests_ ;
    std::deque<QueryResult> requests_ ;
    const static int min_in_day_ = 1440;
    const SearchServer & search_server_;
};

template <typename DocumentPredicate>
std::vector<Document> RequestQueue::AddFindRequest(const std::string & raw_query, DocumentPredicate document_predicate) {
    if ( requests_.size() == min_in_day_) {
        if(requests_.front().count_documents_in_result == 0){
            --count_no_result_requests_;
        }
        requests_.pop_front();
    }
    auto results = search_server_.FindTopDocuments(raw_query, document_predicate);      
    requests_.push_back({raw_query, static_cast<int>(results.size())});
        
    if(results.empty()){
        ++count_no_result_requests_; 
    }
        
    return results;
}