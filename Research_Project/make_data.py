from torch.utils.data import Dataset
import json
import re
class EcoNews(Dataset):
    def __init__(self,path,vocab,tokenizer):
        self.path = path
        self.data = []
        self.vocab = vocab
        self.tokenizer = tokenizer
        self.titleEnd = '<unused0>'
        with open(self.path,encoding='utf-8') as json_file:
            count = 0
            for line in json_file:
                count += 1
                title = json.loads(line)['title']
                text = json.loads(line)['text']
                sum_data = [vocab[vocab.bos_token],] + vocab[tokenizer(title[:-1])] + [vocab[vocab.eos_token]]
                self.data.append(sum_data)
                for i in re.split("[\.?!]\s+",text):
                    sum_data = [vocab[vocab.bos_token],] + vocab[tokenizer(i[:-1])] + [vocab[vocab.eos_token]]
                    if len(sum_data) < 1024:
                        self.data.append(sum_data)
#if count > 10000:
#                   print(len(self.data))
#                   break


#tokenized_text = tokenizer(text[:-1])
#               sum_data = [vocab[vocab.bos_token],] + vocab[tokenized_title] + [vocab.token_to_idx[self.titleEnd]] + vocab[tokenized_text] + [vocab[vocab.eos_token]]
#               self.data.append(sum_data)
    
    def __len__(self):
        return len(self.data)
    def __getitem__(self,index):
        return self.data[index]


