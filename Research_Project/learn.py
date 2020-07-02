import torch
from kogpt2.pytorch_kogpt2 import get_pytorch_kogpt2_model
from gluonnlp.data import SentencepieceTokenizer
from kogpt2.utils import get_tokenizer
from make_data import EcoNews
import torch.nn.functional as F
import json
import re
import random
from torch.utils.data import DataLoader
def top_p(lists, vocab, th):
    item,idx = torch.sort(lists,descending = True)
    idx = idx.tolist()
    item = F.softmax(item,dim=-1)
    sump = 0
    count = 0
    while(sump < th):
        sump += item[count]
        count += 1
    randomNum = random.randint(0,count)
    return vocab.to_tokens(idx[randomNum])

def top_k(lists, vocab, k):
    item,idx = torch.sort(lists,descending = True)
    randomNum = random.randint(0,k)
    idx = idx.tolist()
    return vocab.to_tokens(idx[randomNum])

print(torch.cuda.is_available())
print(torch.cuda.current_device())
print(torch.cuda.device_count())
torch.cuda.set_device('cuda:1')
batch_size = 2
device = torch.device('cuda')
tok_path = get_tokenizer()
model, vocab = get_pytorch_kogpt2_model(ctx='cuda')
model.to(device)
model.train()
tok = SentencepieceTokenizer(tok_path)
print("data Loading Start")
data = EcoNews('train_data.json',vocab,tok)
dataLoader = DataLoader(data,batch_size=2,shuffle=True,pin_memory=True)
print("Data Loading Done")
learning_rate = 1e-5
criterion = torch.nn.CrossEntropyLoss()
optimizer = torch.optim.Adam(model.parameters(), lr=learning_rate)

save_path="./"
print(len(dataLoader))   
for epoch in range(21):
    count = 0
    print(epoch)
    for data in dataLoader:
        optimizer.zero_grad()
        data = torch.stack(data)
        data= data.transpose(1,0)
        data = data.to('cuda')
        outputs = model(data, labels=data)
        loss, logits = outputs[:2]
        loss.backward()
        optimizer.step()
#if count %10 ==0:
#          print('epoch no.{} train no.{}  loss = {}' . format(epoch, count+1, loss))
#          torch.save(model,save_path+'checkpoint_{}_{}.tar'.format(epoch,count))
            # 추론 및 학습 재개를 위한 일반 체크포인트 저장하기
        if (count >0 and count%10000==0 and epoch % 5 == 1 and epoch > 0) or (len(data) < batch_size):
            torch.save({'epoch': epoch,
                        'train_no': count,
                        'model_state_dict': model.state_dict(),
                        'optimizer_state_dict': optimizer.state_dict(),
                        'loss':loss}, save_path+'checkpoint'+str(epoch)+'.tar')

        count += 1
        del data
        torch.cuda.empty_cache()
