#include "request_queue.h" 


RequestQueue::RequestQueue(const SearchServer & search_server):
    count_no_result_requests_(0),
    search_server_(search_server)
    { }

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
RequestQueue::QueryResult::QueryResult(std::string  query_words_, std::vector<Document>  query_result_)
: query_words(query_words_), query_result(query_result_){

}    