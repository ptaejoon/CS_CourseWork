import torch
from kogpt2.pytorch_kogpt2 import get_pytorch_kogpt2_model
from gluonnlp.data import SentencepieceTokenizer
from kogpt2.utils import get_tokenizer
import torch.nn.functional as F
import json
import re
import random
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

device = torch.device('cpu')
ck = 'mediumcheck.tar'
tok_path = get_tokenizer()
model, vocab = get_pytorch_kogpt2_model(ctx='cpu')
checkpoint = torch.load(ck,map_location=device)
model.load_state_dict(checkpoint['model_state_dict'])
tok = SentencepieceTokenizer(tok_path)
print(vocab[vocab.bos_token])
print(vocab.bos_token)
sent_p = '세계 200여 개 기업이 '
sent_k = '세계 200여 개 기업이 '
sent_argmax = '세계 200여 개 기업이 '

    
toked_p = tok(sent_p)
toked_k = tok(sent_k)
toked_argmax = tok(sent_argmax)
sent_cnt = 0
input_ids_p = torch.tensor([vocab[vocab.bos_token],]  + vocab[toked_p]).unsqueeze(0)

count = 0
while True:
    input_ids_p = torch.tensor([vocab[vocab.bos_token],]  + vocab[toked_p]).unsqueeze(0)
    input_ids_k = torch.tensor([vocab[vocab.bos_token],]  + vocab[toked_k]).unsqueeze(0)
    input_ids_argmax = torch.tensor([vocab[vocab.bos_token],]  + vocab[toked_argmax]).unsqueeze(0)
    predict_p = model(input_ids_p)
    predict_k = model(input_ids_k)
    predict_argmax = model(input_ids_argmax)[0]

    last_p = predict_p[0].squeeze()[-1]
    last_k = predict_k[0].squeeze()[-1]
    gen_p = top_p(last_p,vocab,0.1)
    gen_k = top_k(last_k,vocab,10)
    gen_argmax = vocab.to_tokens(torch.argmax(predict_argmax, axis=-1).squeeze().tolist())[-1]
    if count > 100:
        break
    sent_p += gen_p.replace('▁',' ')
    sent_k += gen_k.replace('▁',' ')
    sent_argmax += gen_argmax.replace('▁',' ')
    print(gen_argmax)
    if gen_p == vocab.eos_token:
        sent_p += vocab.bos_token
    if gen_argmax == '</s>':
        sent_argmax += vocab.bos_token
    toked_p = tok(sent_p)
    toked_k = tok(sent_k)
    toked_argmax = tok(sent_argmax)
    count += 1

print("p : ")
print(sent_p)
print("k : ")
print(sent_k)
print("argmax : ")
print(sent_argmax)
"""
#outputs = model.generate(input_ids = input_ids_p, max_length=1024, repetition_penalty=1.2, do_sample=True, eos_token_ids =-1, num_return_sequences=3)

#for i in range(3):
#    toked = vocab.to_tokens(outputs[0][i].squeeze().tolist())
#    ret = re.sub(r'(<s>|</s>)', '' , ''.join(toked).replace('▁', ' ').strip())
#    print('Generated {}: {}'.format(i, ret))

#outputs = model.generate(input_ids=input_ids,max_length=1024, repetition_penalty=1.2, do_sample=True, eos_token_ids=1, num_return_sequences=3)
#for i in range(3): #  3 output sequences were generated
#toked = vocab.to_tokens(outputs[0][i].squeeze().tolist())
#    ret = re.sub(r'(<s>|</s>)', '' , ''.join(toked).replace('▁', ' ').strip())
#    print('Generated {}: {}'.format(i, ret))

sent = '2019년 한해를 보내며,'
toked = tok(sent)
with open("train_data.json") as json_file:
    for line in json_file:
        print(line)
        print(json.loads(line)['text'])
        break
print(toked)

while 1:
    input_ids = torch.tensor([vocab[vocab.bos_token],]  + vocab[toked]).unsqueeze(0)
    pred = model(input_ids)[0]
    gen = vocab.to_tokens(torch.argmax(pred, axis=-1).squeeze().tolist())[-1]
    if gen == '</s>':
        break
    sent += gen.replace('▁', ' ')
    toked = tok(sent)
"""
