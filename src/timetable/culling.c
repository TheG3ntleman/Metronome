#include "culling.h"


static void swap(Cluster* a, Cluster* b) {
    Cluster temp = *a;
    *a = *b;
    *b = temp;
}

// Partition function for Quick Sort
static int partition(Cluster arr[], int low, int high) {
    int pivot = arr[high].size;
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) {
        if (arr[j].size < pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

// Quick Sort algorithm
static void quickSort(Cluster arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);

        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}


// Placeholder for similarity calculation
numeric calculateTimeTableSimilarity(unsigned int schedule1, unsigned int schedule2) {

    // Will be implemented by abhirath

    
    numeric similarity = abs((int)schedule1 - (int)schedule2);
    return similarity;
}

// Initialize a cluster with initial capacity
void initCluster(Cluster* cluster, unsigned int initial_capacity) {
    cluster->schedules = (unsigned int*)malloc(initial_capacity * sizeof(unsigned int));
    cluster->size = 0;
    cluster->capacity = initial_capacity;
}

// Expand the capacity of a cluster if needed
void expandCluster(Cluster* cluster) {
    unsigned int new_capacity = cluster->capacity * 2; // Double the capacity, why? --> Random
    cluster->schedules = (unsigned int*)realloc(cluster->schedules, new_capacity * sizeof(unsigned int));
    cluster->capacity = new_capacity;
}

unsigned int assignToCluster(Population* population, unsigned int schedule_index, Cluster* clusters, unsigned int* num_clusters, unsigned int min_schedules_per_cluster) {
    numeric max_similarity = -10000; // Initializing maximum similarity
    unsigned int closest_cluster = *num_clusters; // Assign to a new cluster initially

    for (unsigned int i = 0; i < *num_clusters; ++i) {
        numeric similarity = calculateTimeTableSimilarity(schedule_index, clusters[i].schedules[0]); // Assuming comparing with the first schedule in the cluster, why first one? cause finding centroid is a pain in the ass and the schedules in a cluster are similar

        if (similarity < SIMILARITY_THRESHOLD) {
            closest_cluster = i; // Assign to a similar cluster if found
            break;
        }
    }

    if (closest_cluster == *num_clusters) {
        // No similar cluster found, create a new cluster --> Happens if the similarity between all the exisiting clusters exceeds the SIMILARITY_THRESHOLD
        initCluster(&clusters[*num_clusters], min_schedules_per_cluster);
        ++(*num_clusters);
    }

    return closest_cluster;
}

void cullPopulation(Population* population, numeric* soft_fitness, numeric* hard_fitness, unsigned int selection_size, unsigned int* selection_arr, unsigned int min_schedules_per_cluster) {
    unsigned int total_schedules = population->number_of_organisms;  // Assuming population size is available
    unsigned int max_clusters = total_schedules / min_schedules_per_cluster;  // Maximum possible clusters

    // Allocate memory for clusters
    Cluster* clusters = (Cluster*)malloc(max_clusters * sizeof(Cluster));
    unsigned int num_clusters = 0;

    // Assign schedules to clusters based on a similarity-based approach
    for (unsigned int i = 0; i < total_schedules; ++i) {
        unsigned int cluster_index = assignToCluster(population, i, clusters, &num_clusters, min_schedules_per_cluster);
        unsigned int cluster_size = clusters[cluster_index].size;

        if (cluster_size >= clusters[cluster_index].capacity) {
            // Expand the cluster capacity if needed
            expandCluster(&clusters[cluster_index]);
        }

        // Add schedule to the cluster
        clusters[cluster_index].schedules[cluster_size] = i;
        clusters[cluster_index].size++;
    }

    

    quickSort(clusters, 0, num_clusters - 1);
    // sort the clusters based on the least number of schedules in each cluster
    for(unsigned int i = 0;i < num_clusters;i++){
        // sort each schedule in cluster[i] in ascending order
        quickSort(clusters[i].schedules, 0, clusters[i].size - 1);
    }

    unsigned int selected_count = 0;

    while (selected_count < selection_size) {
        for (unsigned int i = 0; i < num_clusters; ++i) {
            unsigned int schedules_to_select = clusters[i].size > min_schedules_per_cluster ? min_schedules_per_cluster : clusters[i].size;
            
            for (unsigned int j = 0; j < schedules_to_select && selected_count < selection_size; ++j) {
                selection_arr[selected_count++] = clusters[i].schedules[j];
            }

            clusters[i].size -= schedules_to_select; // Reduce the cluster size after selecting schedules
        }
    }

    // Free allocated memory for clusters
    for (unsigned int i = 0; i < num_clusters; ++i) {
        free(clusters[i].schedules);
    }
    free(clusters);
}
