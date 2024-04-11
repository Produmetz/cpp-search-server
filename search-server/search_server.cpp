#include <cmath>
#include "search_server.h"

template <typename StringContainer>
explicit SearchServer::SearchServer(const StringContainer& stop_words)
    : stop_words_(MakeUniqueNonEmptyStrings(stop_words)) {
    for(const auto &word : stop_words_){
        if(!IsValidWord(word)){
            throw std::invalid_argument("Stop words with special symbol"s);
        }
    }
}

void SearchServer::AddDocument(int document_id, const std:: string & document, DocumentStatus status,
                                   const std::vector<int>& ratings) {
    if(document_id < 0){
        throw std::invalid_argument("Id < 0 "s);
    }
    if(documents_.count(document_id) > 0){
        throw std::invalid_argument("this id was added"s);
    }
        
    const std::vector<std::string> words = SplitIntoWordsNoStop(document);

    ordered_id_.push_back(document_id);

    const double inv_word_count = 1.0 / words.size();
    for (const std::string& word : words) {
        word_to_document_freqs_[word][document_id] += inv_word_count;
    }
    documents_.emplace(document_id, DocumentData{ComputeAverageRating(ratings), status});
}

template <typename DocumentPredicate>
std::vector<Document> SearchServer::FindTopDocuments(const std::string& raw_query, DocumentPredicate document_predicate) const {
    const Query query = ParseQuery(raw_query);
    if((query.minus_words.empty()) && (query.plus_words.empty()) && (!query.with_stop_word)){
        return  {};
    }
    std::vector<Document> matched_documents = FindAllDocuments(query, document_predicate);

    std::sort(matched_documents.begin(), matched_documents.end(),
        [](const Document& lhs, const Document& rhs) {
            if (std::abs(lhs.relevance - rhs.relevance) < EPSILON) {
                return lhs.rating > rhs.rating;
            } else {
                return lhs.relevance > rhs.relevance;
            }
        });
    if (matched_documents.size() > MAX_RESULT_DOCUMENT_COUNT) {
        matched_documents.resize(MAX_RESULT_DOCUMENT_COUNT);
    }
    return matched_documents;
}

std::vector<Document> SearchServer::FindTopDocuments(const std::string& raw_query, DocumentStatus status) const {                                  
    return SearchServer::FindTopDocuments(
        raw_query, [status](int document_id, DocumentStatus document_status, int rating) {
            return document_status == status;
        });
}
std::vector<Document> SearchServer::FindTopDocuments(const std::string& raw_query) const {
    return FindTopDocuments(raw_query, DocumentStatus::ACTUAL);
}

int SearchServer::GetDocumentCount() const {
    return documents_.size();
}

std::tuple<std::vector<std::string>, DocumentStatus> SearchServer::MatchDocument(const std::string& raw_query, int document_id) const {
    const SearchServer::Query query = SearchServer::ParseQuery(raw_query);
        
    std::vector<std::string> matched_words;
    for (const std::string& word : query.plus_words) {
        if (word_to_document_freqs_.count(word) == 0) {
            continue;
        }
        if (word_to_document_freqs_.at(word).count(document_id)) {
            matched_words.push_back(word);
        }
    }
    for (const std::string & word : query.minus_words) {
        if (word_to_document_freqs_.count(word) == 0) {
            continue;
        }
        if (word_to_document_freqs_.at(word).count(document_id)) {
            matched_words.clear();
            break;
        }
    }
    return std::tuple{matched_words, documents_.at(document_id).status};
}

int SearchServer::GetDocumentId(int index) const {
    return ordered_id_.at(index);
}

bool SearchServer::IsStopWord(const std::string& word) const {
        return stop_words_.count(word) > 0;
}
    
std::vector<std::string> SearchServer::SplitIntoWordsNoStop(const std::string& text) const {
    std::vector<std::string> words;
    for (const std::string & word : SplitIntoWords(text)) {
        if (!(IsStopWord(word))) {
            words.push_back(word);
        }
    }
    return words;
}

int SearchServer::ComputeAverageRating(const std::vector<int>& ratings) const{
    if (ratings.empty()) {
        return 0;
    }
    int rating_sum = std::accumulate(ratings.begin(), ratings.end(), 0);
    return rating_sum / static_cast<int>(ratings.size());
}
SearchServer::QueryWord SearchServer::ParseQueryWord(std::string text) const {
    bool is_minus = false;
    if(!IsValidWord(text)){
        throw std::invalid_argument("Query with special symbol"s);
    } 
    if(text[0] == '-'){
        is_minus = true;
        text = text.substr(1);
    }
    if((text[0] == '-') || (text.size() == 0)){
        throw std::invalid_argument("Incorrect use minus in query"s);
    }
        
    return {text, is_minus, IsStopWord(text)};
}
SearchServer::Query SearchServer::ParseQuery(const std::string& text) const {
    Query query;
    for (const std::string & word : SplitIntoWords(text)) {
        const QueryWord query_word = ParseQueryWord(word);
        query.with_stop_word = query_word.is_stop;
        if (!query_word.is_stop) {
            if (query_word.is_minus) {
                query.minus_words.insert(query_word.data);
            } else {
                query.plus_words.insert(query_word.data);
            }
        }
    }
    return query;
}

double SearchServer::ComputeWordInverseDocumentFreq(const std::string & word) const {
    return log(GetDocumentCount() * 1.0 / SearchServer::word_to_document_freqs_.at(word).size());
}

template <typename DocumentPredicate>
std::vector<Document> SearchServer::FindAllDocuments(const Query& query,
                                      DocumentPredicate document_predicate) const {
    std::map<int, double> document_to_relevance;
    for (const std::string& word : query.plus_words) {
        if (word_to_document_freqs_.count(word) == 0) {
            continue;
        }
        const double inverse_document_freq = ComputeWordInverseDocumentFreq(word);
        for (const auto &[document_id, term_freq] : word_to_document_freqs_.at(word)) {
            const auto& document_data = documents_.at(document_id);
            if (document_predicate(document_id, document_data.status, document_data.rating)) {
                document_to_relevance[document_id] += term_freq * inverse_document_freq;
            }
        }
    }

    for (const std::string& word : query.minus_words) {
        if (word_to_document_freqs_.count(word) == 0) {
            continue;
        }
        for (const auto &[document_id, _] : word_to_document_freqs_.at(word)) {
            document_to_relevance.erase(document_id);
        }
    }

    std::vector<Document> matched_documents;
    for (const auto &[document_id, relevance] : document_to_relevance) {
        matched_documents.push_back(
            {document_id, relevance, documents_.at(document_id).rating});
    }
    return matched_documents;
}