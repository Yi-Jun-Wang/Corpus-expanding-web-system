FROM node:16-alpine

WORKDIR /app

COPY . .

RUN npm install -g pnpm \
    && pnpm install --frozen-lockfile

ENV NODE_ENV production

EXPOSE 3000

CMD ["npm", "run", "preview"]