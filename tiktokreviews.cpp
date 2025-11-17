#include <iostream>
#include <fstream>
#include <string>
#include <cctype>

using namespace std;

const int MAX_REVIEWS = 100;
const string DATA_FILE = "tiktok_reviews.txt";

struct Review {
    string trend;
    int rating;
    string reviewer;
};

string toLower(string s) {
    for (char &c : s) c = (char)tolower(c);
    return s;
}

int loadReviews(Review reviews[]) {
    ifstream in(DATA_FILE);
    if (!in) return 0;
    int count = 0;
    string line;
    while (getline(in, line) && count < MAX_REVIEWS) {
        if (line.empty()) continue;
        size_t lastSpace = line.find_last_of(' ');
        if (lastSpace == string::npos) continue;
        string reviewer = line.substr(lastSpace + 1);
        string beforeReviewer = line.substr(0, lastSpace);
        size_t secondLast = beforeReviewer.find_last_of(' ');
        if (secondLast == string::npos) continue;
        string ratingStr = beforeReviewer.substr(secondLast + 1);
        string trend = beforeReviewer.substr(0, secondLast);
        reviews[count].trend = trend;
        reviews[count].rating = stoi(ratingStr);
        reviews[count].reviewer = reviewer;
        count++;
    }
    return count;
}

void appendReview(const Review &r) {
    ofstream out(DATA_FILE, ios::app);
    if (!out) {
        cout << "Error opening file.\n";
        return;
    }
    out << r.trend << " " << r.rating << " " << r.reviewer << "\n";
}

void viewAll(const Review reviews[], int count) {
    if (count == 0) {
        cout << "No reviews found.\n";
        return;
    }
    cout << "TikTok Trend               Rating  Reviewer\n";
    cout << "-------------------------------------------\n";
    for (int i = 0; i < count; ++i) {
        cout << reviews[i].trend << "  " << reviews[i].rating
             << "  " << reviews[i].reviewer << "\n";
    }
}

void searchReview(const Review reviews[], int count) {
    if (count == 0) {
        cout << "No reviews to search.\n";
        return;
    }
    cout << "Enter trend/influencer name: ";
    string q;
    getline(cin, q);
    string qLower = toLower(q);
    bool found = false;
    for (int i = 0; i < count; ++i) {
        if (toLower(reviews[i].trend) == qLower) {
            cout << "Trend: " << reviews[i].trend
                 << ", Rating: " << reviews[i].rating
                 << ", Reviewed by: " << reviews[i].reviewer << "\n";
            found = true;
        }
    }
    if (!found) cout << "No review found for \"" << q << "\".\n";
}

void addReview(Review reviews[], int &count) {
    if (count >= MAX_REVIEWS) {
        cout << "Database full.\n";
        return;
    }
    Review r;
    cout << "Enter trend/influencer name: ";
    getline(cin, r.trend);
    cout << "Enter rating (1-10): ";
    cin >> r.rating;
    cin.ignore(1000, '\n');
    cout << "Enter reviewer name: ";
    getline(cin, r.reviewer);

    reviews[count++] = r;
    appendReview(r);
    cout << "Review added.\n";
}

int main() {
    Review reviews[MAX_REVIEWS];
    int reviewCount = loadReviews(reviews);
    int choice;

    do {
        cout << "\nWelcome to the TikTok Review Database\n";
        cout << "1. View All Reviews\n";
        cout << "2. Search for a Trend/Influencer\n";
        cout << "3. Add a New Review\n";
        cout << "4. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(1000, '\n');

        if (choice == 1) {
            viewAll(reviews, reviewCount);
        } else if (choice == 2) {
            searchReview(reviews, reviewCount);
        } else if (choice == 3) {
            addReview(reviews, reviewCount);
        } else if (choice == 4) {
            cout << "Goodbye!\n";
        } else {
            cout << "Invalid choice.\n";
        }
    } while (choice != 4);

    return 0;
}