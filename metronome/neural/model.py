import torch
from torch import nn
from torch.functional import F

from transformers import AlbertConfig, AlbertModel

import numpy as np


# Configuring the albert model as necessary

class SelfAttentionPooling(nn.Module):
    def __init__(self, hidden_size):
        super(SelfAttentionPooling, self).__init__()
        self.hidden_size = hidden_size
        self.query = nn.Linear(hidden_size, hidden_size)
        self.key = nn.Linear(hidden_size, hidden_size)
        self.value = nn.Linear(hidden_size, hidden_size)
        self.softmax = nn.Softmax(dim=-1)

    def forward(self, hidden_states):
        # Compute queries, keys, and values
        queries = self.query(hidden_states)
        keys = self.key(hidden_states)
        values = self.value(hidden_states)

        # Compute attention scores
        attention_scores = torch.matmul(queries, keys.transpose(-2, -1)) / torch.sqrt(torch.tensor(self.hidden_size, dtype=torch.float32))

        # Apply softmax to obtain attention weights
        attention_weights = self.softmax(attention_scores)

        # Compute weighted sum using attention weights
        pooled_output = torch.matmul(attention_weights, values)

        return pooled_output

class ControlNetwork(nn.Module):
  
  def __init__(self, vocabulary_size : int = 44, maximum_input_length : int = 256, hidden_size : int = 512):
    super().__init__()
    
    self.maximum_input_length = maximum_input_length
    self.hidden_size = hidden_size
    
    self.albert_config = AlbertConfig(
			vocab_size = vocabulary_size + 4, # 0 - pad, 1 - bos, 2 - eos, 3 - sep
			embedding_size = 32, # Small embedding size as the vocabulary size is small
      hidden_size = hidden_size, # for now hyper-parameter optimization can take care of this later.
      num_hidden_layers = 12,
      num_hidden_groups = 1, # Number of groups where parameters are shared.
      num_attention_heads = 64,
      intermediate_size = 2048,
      inner_group_num = 1,
      hidden_act = "gelu",
      hidden_dropout_prob = 0.05,
      attention_probs_dropout_prob = 0.0,
      max_position_embeddings = maximum_input_length,
      pad_token_id = 0,
      bos_token_id = 1,
      eos_token_id = 2,
		)
    
    self.albert = AlbertModel(self.albert_config, add_pooling_layer = True)
    self.final_self_attention = SelfAttentionPooling(hidden_size)
    #self.final_pooling = torch.mean(pooler_output, dim=0)
    self.ffnn = nn.Sequential(
        nn.Linear(hidden_size, 256),
        nn.GELU(),
        nn.Linear(256, 128),
        nn.GELU(),
        nn.Linear(128, 101)
    )

  def string_formater(self, string, eos = 2, bos = 1, pad = 0, sep = 3):
      
      # Converting string to numpy array and adding 4 to each element
      string = np.array(string)
      string += 4

      # reducing string_chunk to accommodate for eos, bos, sep, etc
      maximum_string_chunk = self.maximum_input_length - 2

      # List to store divided strings
      divided_strings = []

      # Divide the modified string into smaller chunks
      num_chunks = len(string) // maximum_string_chunk + (1 if len(string) % maximum_string_chunk != 0 else 0)
      for i in range(num_chunks):
          start_index = i * maximum_string_chunk
          end_index = min((i + 1) * maximum_string_chunk, len(string))
          chunk = string[start_index:end_index]

          # Add appropriate bos, sep, eos elements
          if i == 0:
              chunk = np.concatenate(([bos], chunk, [sep]))
          elif i == num_chunks - 1:
              chunk = np.concatenate(([sep], chunk, [eos]))
          else:
              chunk = np.concatenate(([sep], chunk, [sep]))

          # Pad the last chunk if necessary
          if len(chunk) < maximum_string_chunk + 2:
              pad_size = maximum_string_chunk + 2 - len(chunk)
              chunk = chunk[:-1]
              chunk = np.concatenate((chunk, [pad] * pad_size))
              chunk = np.concatenate((chunk, [eos]))

          # Add the chunk to the list
          divided_strings.append(chunk)

      # Convert the list of divided strings into a numpy array
      result_array = torch.tensor(divided_strings, device = "cuda:0")

      return result_array	
  
  def forward(self, x):
    albert_output = self.albert(x).pooler_output
    contextualized_output = self.final_self_attention(albert_output)
    
    ffnn_input = torch.mean(contextualized_output, dim = 0)
    return self.ffnn(ffnn_input)
    
    #self.final_pooling(self.albert(x).pooler_output)
    
def get_info(self):
    # Returns hidden size and maximum input length
    return self.hidden_size, self.maximum_input_length
