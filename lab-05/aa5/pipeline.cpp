#include "pipeline.h"

pipeTask* gener(pipeTask* pt) {
    pt->start_generate = chrono::high_resolution_clock::now();

    auto k1 = generate();
    pt->m1 = get_csrrepresent_m(k1);
    k1 = generate();
    pt->m2 = get_csrrepresent_m(k1);

    pt->stop_generate = chrono::high_resolution_clock::now();

    return pt;
}

pipeTask* multic(pipeTask* pt) {
    pt->start_mul = chrono::high_resolution_clock::now();

    pt->mrez = mutl((pt->m1), (pt->m2), N);

    pt->stop_mul = chrono::high_resolution_clock::now();

    return pt;
}

pipeTask* write(pipeTask* pt) {
    pt->start_write = chrono::high_resolution_clock::now();

    ofstream out("out.txt");


    out << '\n';
    for (int j = 0; j < int(pt->mrez->rows_index.size()); j++)
        out << pt->mrez->rows_index[j] << ' ';
    out << '\n';
    for (int j = 0; j < int(pt->mrez->columns.size()); j++)
        out << pt->mrez->columns[j] << ' ';
    out << '\n';
    for (int j = 0; j < int(pt->mrez->elements.size()); j++)
        out << pt->mrez->elements[j] << ' ';
    out << '\n';

    out.close();

    pt->stop_write = chrono::high_resolution_clock::now();

    return pt;
}


void gener_conv(queue* qu, queue* qu2, int l) {
    int ll = 0;
    while (1) {
        if (qu->queue.size() > 0) {
            auto k = gener(qu->queue[0]);
            qu->queue.erase(qu->queue.begin());
            qu2->queue.push_back(k);
            ll++;
        }

        if (ll == l)
            break;            
    }
}

void multic_conv(queue* qu, queue* qu3, int l) {
    int ll = 0;
    while (1) {
        if (qu->queue.size() > 0) {
            auto k = multic(qu->queue[0]);
            qu->queue.erase(qu->queue.begin());
            qu3->queue.push_back(k);
            ll++;
        }

        if (ll == l)
            break; 
    }
}

void write_conv(queue* qu, queue* quf, int l) {
    int ll = 0;
    while (1) {
        if (qu->queue.size() > 0) {
            auto k = write(qu->queue[0]);
            qu->queue.erase(qu->queue.begin());
            quf->queue.push_back(k);
            ll++;
        }

        if (ll == l)
            break; 
    }
}

queue* pipeline(int count) {
    vector<thread> threads(3);

    queue* qu1 = new(queue);
    queue* qu2 = new(queue);
    queue* qu3 = new(queue);
    queue* quf = new(queue);

    qu1->stop = false;
    qu2->stop = false;
    qu3->stop = false;

    threads[0] = thread(gener_conv, qu1, qu2, count);
    threads[1] = thread(multic_conv, qu2, qu3, count);
    threads[2] = thread(write_conv, qu3, quf, count);

    for (int i = 0; i < count; i++) {
        auto pt = new(pipeTask);
        pt->n = i;
        qu1->queue.push_back(pt);
    }

    for (auto& thr: threads)
        thr.join();
    
    return quf;
}

queue* pipelineposl(int count) {
    queue* quf = new(queue);

    for (int i = 0; i < count; i++) {
        auto pt = new(pipeTask);
        pt->n = i;
        pt = gener(pt);
        pt = multic(pt);
        pt = write(pt);
        quf->queue.push_back(pt);
    }
    
    return quf;
}