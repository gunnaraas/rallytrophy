#!/usr/bin/env python3
"""List instruction pairs of NONMATCHING functions that differ only in one number (field offset or constant).

A wrong struct offset, struct size or constant in the source shows up as an otherwise identical instruction with a
different displacement/immediate. Run after match.py and `triage.py --md analysis/nonmatching-triage.md`.
Many hits are harmless (a pointer anchored at another field); check the base register before trusting one.

usage: numdiff.py
"""
import subprocess,re,collections
rows=[]
import os
for l in open(os.path.join(os.path.dirname(os.path.dirname(os.path.abspath(__file__))), 'analysis', 'nonmatching-triage.md')):
    c=[x.strip() for x in l.split('|')]
    if len(c)>9 and c[1].startswith('0x'): rows.append((c[1],c[2],c[3],int(c[7])))
num=re.compile(r'0x[0-9a-f]+')
for a,f,n,d in rows:
    out=subprocess.run(['python3','-I','tools/insdiff.py',f,a],capture_output=True,text=True).stdout.splitlines()
    hunks=[];cur=None
    for l in out:
        if l.startswith('@@'): cur=([],[]);hunks.append(cur)
        elif cur is not None and l.startswith('-') : cur[0].append(l[1:])
        elif cur is not None and l.startswith('+') : cur[1].append(l[1:])
    found=collections.Counter()
    for m,p in hunks:
        if len(m)!=len(p) or len(m)>3: continue
        for x,y in zip(m,p):
            if x==y: continue
            if num.sub('N',x)!=num.sub('N',y): continue
            if 'esp' in x or 'ebp' in x or x.split()[0].startswith('j') or x.startswith('call'): continue
            found[(x,y)]+=1
    for (x,y),k in found.items(): print(a,n,d,'|',x,'  =>  ',y,'x%d'%k)
