from frolvlad/alpine-miniconda3:latest

WORKDIR /workspace

COPY environment.yaml \
     .env \
     ACCOUNT.json \
     127.0.0.1-key.pem \
     127.0.0.1.pem \
     API_test.py \
     /workspace

RUN conda env create -f environment.yaml

ENV PATH=/opt/conda/envs/flask/bin:$PATH

EXPOSE 5000

CMD ["python", "API_test.py"]