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
    std::vector<Document> AddFindRequest(const std::string & raw_query, DocumentPredicate document_predicate) {
        if ( requests_.size() == min_in_day_) {
            if(requests_.front().query_result.empty()){
                --count_no_result_requests_;
            }
            requests_.pop_front();
        }
        
        requests_.push_back({raw_query, search_server_.FindTopDocuments(raw_query, document_predicate)});
        
        if(requests_.back().query_result.empty()){
            ++count_no_result_requests_; 
        }
        
        return requests_.back().query_result;
    }
    std::vector<Document> AddFindRequest(const std::string& raw_query, DocumentStatus status);
    std::vector<Document> AddFindRequest(const std::string& raw_query);
    int GetNoResultRequests() const;
private:
    struct QueryResult {
        QueryResult() ;
        QueryResult(std::string  query_words_, std::vector<Document>  query_result_);
        
        std::string query_words = ""s;
        std::vector<Document> query_result = {};
    };
    int count_no_result_requests_ ;
    std::deque<QueryResult> requests_ ;
    const static int min_in_day_ = 1440;
    const SearchServer & search_server_;
};