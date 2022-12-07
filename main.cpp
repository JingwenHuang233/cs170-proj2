#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <math.h>

using namespace std;
double leave_one_out_cross_validation(vector<vector<double>> data, vector<int> current_set, int feature_to_add_or_remove, int mode);
void forward(vector<vector<double>> data);
void backward(vector<vector<double>> data);
vector<double> subtract(vector<double>object_to_classify, vector<double>other);
vector<double> power_of_2(vector<double>);
double sum(vector<double>);
int main(int argc, char** argv){
    ifstream file;  //https://linuxhint.com/read-text-file-2d-array-cpp/
    file.open(argv[1]);
    vector<vector<double>> data;
    string line;
    while(getline(file, line)){
        string entry;
        stringstream ss(line);
        vector<double> v;
        while(ss>>entry){
            v.push_back(stod(entry));
        }
        data.push_back(v);

    }
    file.close();
    int choice;
    cout<<"Choose search algorithm:\n1-Forward Selection\n2-Backward Elimination\n";
    cin>>choice;
    switch(choice){
        case 1: 
            forward(data);
            break;
        case 2:
            backward(data);
            break;
    }
    return 0;
}

double leave_one_out_cross_validation(vector<vector<double>> data, vector<int> current_set, int feature_to_add_or_remove, int mode){
    vector<vector<double>> d(data.size());
    if(mode == 1){
        current_set.push_back(feature_to_add_or_remove);
        for(int i = 0; i<data.size(); ++i){
            d[i].push_back(data[i][0]);
            for(int j = 0; j<current_set.size(); ++j){
                d[i].push_back(data[i][current_set[j]]);
            }
        }
    }
    else if(mode == 2){
        if(feature_to_add_or_remove == 0){
            d = data;
        }
        else{
            current_set.erase(find(current_set.begin(), current_set.end(), feature_to_add_or_remove));
            for(int i = 0; i<data.size(); ++i){
                d[i].push_back(data[i][0]);
                for(int j = 0; j<current_set.size(); ++j){
                    d[i].push_back(data[i][current_set[j]]);
                }
            }
        }
    }
    
    int correct = 0;
    double accuracy = 0;
    for(int i = 0; i<d.size(); ++i){
        vector<double> object_to_classify(d[i].begin()+1, d[i].end());
        int label = d[i][0];
        double nearest_neighbor_distance = numeric_limits<double>::infinity();
        double nearest_neighbor_location = numeric_limits<double>::infinity();
        int nearest_neighbor_label = 0;
        for(int j = 0; j<d.size(); ++j){
            if(j!=i){
                vector<double> other(d[j].begin()+1, d[j].end());
                double distance = sqrt(sum(power_of_2(subtract(object_to_classify, other))));
                if(distance< nearest_neighbor_distance){
                    nearest_neighbor_distance = distance;
                    nearest_neighbor_location = j;
                    nearest_neighbor_label = d[nearest_neighbor_location][0];
                }
            }
            
        }
        // cout<<"Object "<< i+1 << " is in class "<< label << endl;
        // cout<< "Its nearest neighbor is "<< nearest_neighbor_location<< " which is in class "<< nearest_neighbor_label << endl;
        if(label == nearest_neighbor_label){
            ++correct;
        }
    }
    accuracy = (double)correct/data.size();
    cout<<"Accuracy: "<<accuracy<<endl;
    return accuracy;
}

void forward(vector<vector<double>> data){
    int numFeatures = data[0].size();
    vector<int> current_set_of_features;
    double best_accuracy = 0;
    vector<int> best_features_set;
    for (int i = 1; i< numFeatures; ++i){
        cout<< "On the "<<i<<" th level of of the search tree"<<endl;
        int feature_to_add_at_this_level;
        double best_so_far_accuracy = 0;
        for (int j = 1; j< numFeatures; ++j){
            if (find(current_set_of_features.begin(), current_set_of_features.end(), j) == current_set_of_features.end()){
                cout<< "--Considering adding feature "<<j<<endl;
                double accuracy = leave_one_out_cross_validation(data,current_set_of_features,j,1);
                if (accuracy > best_so_far_accuracy){
                    best_so_far_accuracy = accuracy;
                    feature_to_add_at_this_level = j;
                }
            }
        }
        current_set_of_features.push_back(feature_to_add_at_this_level);
        cout<< "On level "<<i<<", I added feature "<< feature_to_add_at_this_level<<endl;
        if(best_so_far_accuracy>best_accuracy){
            best_accuracy = best_so_far_accuracy;
            best_features_set = current_set_of_features;
        }
    }
    cout<<"The best feature set is {"<<best_features_set[0];
    for(int i = 1; i<best_features_set.size(); i++){
        cout<<","<<best_features_set[i];
    }
    cout<<"}, which has an accuracy of "<<best_accuracy*100<<"%\n";
}

void backward(vector<vector<double>> data){
    int numFeatures = data[0].size();
    vector<int> current_set_of_features;
    for (int i = 1; i < numFeatures; i++) {
        current_set_of_features.push_back(i);
    }
    double best_accuracy = leave_one_out_cross_validation(data,current_set_of_features,0,2);//no feature to remove, need to fix
    vector<int> best_features_set = current_set_of_features;
    for (int i = 1; i< numFeatures; ++i){
        cout<< "On the "<<i<<" th level of of the search tree"<<endl;
        int feature_to_remove_at_this_level;
        double best_so_far_accuracy = 0; 
        for (int j = 1; j< numFeatures; ++j){
            if (find(current_set_of_features.begin(), current_set_of_features.end(), j) != current_set_of_features.end()){ //if found
                cout<< "--Considering removing feature "<<j<<endl;
                double accuracy = leave_one_out_cross_validation(data,current_set_of_features,j,2);
                if (accuracy > best_so_far_accuracy){
                    best_so_far_accuracy = accuracy;
                    feature_to_remove_at_this_level = j;
                }
            }
        }
        current_set_of_features.erase(find(current_set_of_features.begin(), current_set_of_features.end(), feature_to_remove_at_this_level));
        cout<< "On level "<<i<<", I removeded feature "<< feature_to_remove_at_this_level<<endl;
        cout<<"best Accuracy: "<<best_accuracy<<endl;
        cout<<"Accuracy at this level: "<<best_so_far_accuracy<<endl;
        if(best_so_far_accuracy>best_accuracy){
            best_accuracy = best_so_far_accuracy;
            best_features_set = current_set_of_features;
        }
    }
    cout<<"The best feature set is {"<<best_features_set[0];
    for(int i = 1; i<best_features_set.size(); i++){
        cout<<","<<best_features_set[i];
    }
    cout<<"}, which has an accuracy of "<<best_accuracy*100<<"%\n";

}

vector<double> subtract(vector<double>object_to_classify, vector<double>other){
    vector<double> output;
    for (int i = 0; i<object_to_classify.size(); i++){
        output.push_back(object_to_classify[i]-other[i]);
    }
    return output;
}
vector<double> power_of_2(vector<double> a){
    vector<double> output;
    for(int i = 0; i<a.size(); ++i){
        output.push_back(a[i]*a[i]);
    }
    return output;
}
double sum(vector<double> a){
    double sum = 0;
    for(int i = 0; i<a.size(); ++i){
        sum+=a[i];
    }
    return sum;
}