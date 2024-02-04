#include "culling.h"
#include <stdio.h>

// Helper functions for quicksort algorithm

static void swap(Cluster *a, Cluster *b) {
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

// Initializing a cluster with some capacity
static void initializeCluster(Cluster *cluster, uint initial_capacity) {
  cluster->timetable_indices = (uint *)malloc(initial_capacity * sizeof(uint));
  cluster->size = 0;
  cluster->capacity = initial_capacity;
}

// Expanding the capacity of an existing cluster if needed
static void expandCluster(Cluster *cluster) {
  // Doubling capacity
  uint new_capacity = cluster->capacity * 2;

  cluster->timetable_indices =
      (uint *)realloc(cluster->timetable_indices, new_capacity * sizeof(uint));
  cluster->capacity = new_capacity;
}

static uint getMostSimilarCluster(Population *population, uint timetable_index,
                                  Cluster *clusters, uint *n_clusters,
                                  GeneticSpecifications *gaSpecs) {

  uint maximum_similarity = -10000;
  uint closest_cluster = *n_clusters; // Assigning next cluster initially.
  //
  for (uint i = 0; i < *n_clusters; i++) {
    numeric similarity = ttCalculateSimilarity(
        population, timetable_index, clusters[i].timetable_indices[0]);

    if (similarity >= gaSpecs->similarity_threshold) {
      // If the similrity
      closest_cluster = i;
      break;
    }
  }

  // In case no existing cluster is close enough to this timetable then, we make
  // a new cluster for it.
  if (closest_cluster == *n_clusters) {
    initializeCluster(clusters + *n_clusters,
                      gaSpecs->minimum_timetables_per_cluster);
    ++(*n_clusters);
  }

  return closest_cluster;
}

void cullPopulationViaClustering(Population *population, numeric *soft_fitness,
                                 numeric *hard_fitness,
                                 uint *indices_of_selected_timetables,
                                 GeneticSpecifications *gaSpecs) {
  // Making some calculations about the dimensions of the
  // problem.
  uint max_clusters =
      population->n_timetables / gaSpecs->minimum_timetables_per_cluster;

  // Allocating memory for clusters
  // For now, we will be allocating memory for the maximum
  // number of possible clusters.
  Cluster *clusters = (Cluster *)malloc(max_clusters * sizeof(Cluster));
  // Variable to keep track of number of actual active clusters
  uint n_clusters = 0;

  // Assigning timetables to clusters based on similarity.
  for (uint i = 0; i < population->n_timetables; ++i) {
    uint cluster_index =
        getMostSimilarCluster(population, i, clusters, &n_clusters, gaSpecs);
    uint cluster_size = clusters[cluster_index].size;

    if (cluster_size >= clusters[cluster_index].capacity) {
      expandCluster(clusters + cluster_index);
    }

    // Add timetable to cluster
    clusters[cluster_index].timetable_indices[cluster_size] = i;
    clusters[cluster_index].size++;
  }

  quickSort(clusters, 0, n_clusters - 1);
  // Sorting clusters based on the least number of schedules in each cluster
  for (uint i = 0; i < n_clusters; i++)
    // Sort each timetable in cluster[i] in ascending order.
    quickSort(clusters[i].timetable_indices, 0, clusters[i].size - 1);

  uint selected_count = 0;

  while (selected_count < gaSpecs->selection_size) {
    for (uint i = 0; i < n_clusters; i++) {
      uint schedule_to_select =
          clusters[i].size > gaSpecs->minimum_timetables_per_cluster
              ? gaSpecs->minimum_timetables_per_cluster
              : clusters[i].size;

      for (uint j = 0;
           j < schedule_to_select && selected_count < gaSpecs->selection_size;
           j++) {
        indices_of_selected_timetables[selected_count++] =
            clusters[i].timetable_indices[j];
      }

      // Reduce the cluster size after selecting those timetables. That way we
      // won't select them again since they are sorted.
      clusters[i].size -= schedule_to_select;
    }
  }

  // Freeing allocated memory
  for (uint i = 0; i < n_clusters; i++) {
    free(clusters[i].timetable_indices);
  }
  free(clusters);
}

// Helper function for selection based culling

static char isIn(uint x, uint *arr, uint size) {

  for (uint i = 0; i < size; i++) {
    if (arr[i] == x)
      return 1;
  }

  return 0;
}

void cullPopulationViaSelection(Population *population, numeric *soft_fitness,
                                numeric *hard_fitness,
                                uint *indices_of_selected_timetables,
                                GeneticSpecifications *gaSpecs) {

  // Calcuting normed fitness values
  numeric normed_fitness[population->n_timetables];

  for (uint i = 0; i < population->n_timetables; i++)
    normed_fitness[i] = norm_l2(soft_fitness[i], hard_fitness[i]);

  // Selecting top timetable indices based on highest normed fitness

  for (uint i = 0; i < gaSpecs->selection_size; i++) {

    uint index = 0;
    numeric max_fitness = 0;
    // Iterating through all normed fitness

    for (uint j = 0; j < population->n_timetables; j++) {

      // Recording timetable with highest fitness that is not already selected.
      if (normed_fitness[j] > max_fitness &&
          !isIn(j, indices_of_selected_timetables, i)) {
        index = j;
        max_fitness = normed_fitness[j];
      }
    }

    indices_of_selected_timetables[i] = index;
  }
}
