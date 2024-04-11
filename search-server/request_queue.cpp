#include "request_queue.h" 


RequestQueue::RequestQueue(const SearchServer & search_server):
    count_no_result_requests_(0),
    search_server_(search_server)
    { }

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

std::vector<Document> RequestQueue::AddFindRequest(const std::string & raw_query, DocumentStatus status) {
    return RequestQueue::AddFindRequest(
        raw_query, [status](int document_id, DocumentStatus document_status, int rating) {
            return (document_status == status);
        });
        
}
std::vector<Document> RequestQueue::AddFindRequest(const std::string & raw_query) {
    return RequestQueue::AddFindRequest(raw_query, DocumentStatus::ACTUAL);
}
int RequestQueue::GetNoResultRequests() const {
    return RequestQueue::count_no_result_requests_;
}
RequestQueue::QueryResult::QueryResult() = default;
RequestQueue::QueryResult::QueryResult(std::string  query_words_, int count_documents_in_result_)
: query_words(query_words_), count_documents_in_result(count_documents_in_result_){

}    